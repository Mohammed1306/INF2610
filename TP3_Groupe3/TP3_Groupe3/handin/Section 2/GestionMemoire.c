#include<stdbool.h>
#include "./libs/lib.h"

const unsigned int TAILLE_PAGE = 1024;

unsigned int calculerNumeroDePage(unsigned long adresse) {
    return adresse / TAILLE_PAGE;
}

unsigned long calculerDeplacementDansLaPage(unsigned long adresse) {
    return adresse % TAILLE_PAGE;
}

unsigned long calculerAdresseComplete(unsigned int numeroDePage, unsigned long deplacementDansLaPage) {
    return numeroDePage * TAILLE_PAGE + deplacementDansLaPage;
}

void rechercherTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	unsigned int numeroPageVirtuelle = calculerNumeroDePage(req->adresseVirtuelle);
    for (int i = 0; i < TAILLE_TLB; i++) {
        if (mem->tlb->entreeValide[i] && mem->tlb->numeroPage[i] == numeroPageVirtuelle) {
            unsigned int numeroCadre = mem->tlb->numeroCadre[i];
            unsigned long deplacement = calculerDeplacementDansLaPage(req->adresseVirtuelle);
            req->adressePhysique = calculerAdresseComplete(numeroCadre, deplacement);
            mem->tlb->dernierAcces[i] = req->date;
            req->estDansTLB = 1;
            return;
        }
    }
    req->adressePhysique = 0;  
    req->estDansTLB = 0;    
}

void rechercherTableDesPages(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	unsigned int numeroPageVirtuelle = calculerNumeroDePage(req->adresseVirtuelle);
	unsigned int numeroCadre = mem->tp->numeroCadre[numeroPageVirtuelle];
	if (mem->tp->entreeValide[numeroPageVirtuelle]) {
		unsigned long deplacement = calculerDeplacementDansLaPage(req->adresseVirtuelle);
		req->adressePhysique = calculerAdresseComplete(numeroCadre, deplacement);
		req->estDansTablePages = 1;
	} else {
		req->adressePhysique = 0;  
    	req->estDansTablePages = 0; 
	}
}

void ajouterDansMemoire(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
    unsigned int numeroPage = calculerNumeroDePage(req->adresseVirtuelle);

    for (unsigned int i = 0; i < TAILLE_MEMOIRE; i++) {
        if (!mem->memoire->utilisee[i]) {  
            mem->memoire->numeroPage[i] = numeroPage;
            mem->memoire->dernierAcces[i] = req->date;
            mem->memoire->dateCreation[i] = req->date;
            mem->memoire->utilisee[i] = 1;
            mem->tp->numeroCadre[numeroPage] = i;
            mem->tp->entreeValide[numeroPage] = 1;
            unsigned long deplacement = calculerDeplacementDansLaPage(req->adresseVirtuelle);
            req->adressePhysique = calculerAdresseComplete(i, deplacement);

            return; 
        }
    }
    req->adressePhysique = 0;
}


void mettreAJourTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
    unsigned int numeroPage = calculerNumeroDePage(req->adresseVirtuelle);
    unsigned int cadrePhysique = calculerNumeroDePage(req->adressePhysique); 

    for (int i = 0; i < TAILLE_TLB; i++) {
        if (!mem->tlb->entreeValide[i]) {  
            mem->tlb->numeroPage[i] = numeroPage;
            mem->tlb->numeroCadre[i] = cadrePhysique;
            mem->tlb->dernierAcces[i] = req->date;
            mem->tlb->dateCreation[i] = req->date;
            mem->tlb->entreeValide[i] = 1;
            return;
        }
    }

    int entreeRemplacement = 0; 
    unsigned long dateCreationPlusAncienne = mem->tlb->dateCreation[0];

    for (int i = 1; i < TAILLE_TLB; i++) {
        if (mem->tlb->dateCreation[i] < dateCreationPlusAncienne) {
            entreeRemplacement = i;
            dateCreationPlusAncienne = mem->tlb->dateCreation[i];
        }
    }

    mem->tlb->numeroPage[entreeRemplacement] = numeroPage;
    mem->tlb->numeroCadre[entreeRemplacement] = cadrePhysique;
    mem->tlb->dernierAcces[entreeRemplacement] = req->date;
    mem->tlb->dateCreation[entreeRemplacement] = req->date;
    mem->tlb->entreeValide[entreeRemplacement] = 1; 
}

// NE PAS MODIFIER
int main() {
    evaluate(
		&calculerNumeroDePage, 
		&calculerDeplacementDansLaPage, 
		&calculerAdresseComplete, 
        &rechercherTLB, 
		&rechercherTableDesPages,
		&mettreAJourTLB,
		&ajouterDansMemoire
    );
    return 0;
}
