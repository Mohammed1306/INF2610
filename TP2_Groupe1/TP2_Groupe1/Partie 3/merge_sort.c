#include "merge_sort.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <array_size> <num_processes>\n", argv[0]);
        exit(1);
    }


    int num_processes = atoi(argv[1]);
    int array_size = atoi(argv[2]);

    if (num_processes < 1 || array_size < 1 || array_size > MAX_ARRAY_SIZE) {
        fprintf(stderr, "Le nombere de processus doit etre supérieur ou égale a 1 et la taille du tableau entre 1 et %d.\n", MAX_ARRAY_SIZE);
        exit(EXIT_FAILURE);
    }

    fd = shm_open("/shared_data", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Erreur lors de la creation de la memoire paratgé");
        exit(EXIT_FAILURE);
    }

    size_t shared_mem_size = sizeof(SharedData) + array_size * sizeof(int);
    if (ftruncate(fd, shared_mem_size) == -1) {
        perror("Erreur lors de ftruncate");
        exit(EXIT_FAILURE);
    }

    shared_data = mmap(NULL, shared_mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("Erreur lors de mmap");
        exit(EXIT_FAILURE);
    }

    shared_data -> size = array_size;
    shared_data -> array = (int*)(shared_data + 1);

    /* Populate the array to test the sort */
    srand(time(NULL));
    for (int i = 0; i < array_size; i++) {
        shared_data->array[i] = rand() % MAX_NUM_SIZE;
    }

    execute_merge_sort(0, array_size - 1, num_processes);

    while (wait(NULL) > 0);

    show_array();

    munmap(shared_data, shared_mem_size);
    shm_unlink("/shared_data");

    return 0;
}

void merge_sort( int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(left, mid);
        merge_sort(mid + 1, right);
        merge(left, mid, right);
    }
}

void merge(int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = shared_data->array[left + i];
    for (j = 0; j < n2; j++)
        R[j] = shared_data->array[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            shared_data->array[k] = L[i];
            i++;
        } else {
            shared_data->array[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        shared_data->array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        shared_data->array[k] = R[j];
        j++;
        k++;
    }
}

void execute_merge_sort(int start, int end, int num_processes) {
    if (num_processes <= 1 || end - start <= 0) {
        merge_sort(start, end);
    } else {
        int mid = start + (end - start) / 2;
        pid_t left_pid = fork();
        if (left_pid < 0) {
            perror("Erreur lors du fork gauche");
            exit(EXIT_FAILURE);
        } else if (left_pid == 0) {
            execute_merge_sort(start, mid, num_processes / 2);
            _exit(0);
        } else {
            pid_t right_pid = fork();
            if (right_pid < 0) {
                perror("Erreur lors du fork droit");
                exit(EXIT_FAILURE);
            } else if (right_pid == 0) {
                execute_merge_sort(mid + 1, end, num_processes - num_processes / 2);
                _exit(0);
            } else  {
                waitpid(left_pid, NULL, 0);
                waitpid(right_pid, NULL, 0);
                merge(start, mid, end);
            }
        }
    }
}

void show_array(){
    printf("Sorted array: ");
    for (int i = 0; i < shared_data->size; i++) {
        printf("%d ", shared_data->array[i]);
    }
    printf("\n");
}