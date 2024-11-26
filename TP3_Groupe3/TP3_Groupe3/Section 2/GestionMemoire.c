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
	//TODO
}

void mettreAJourTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	// TODO
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
