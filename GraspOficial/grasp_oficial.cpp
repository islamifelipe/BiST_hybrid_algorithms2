

#ifndef GRASP_CPP
#define GRASP_CPP

/* Implementacao fornecida por silvia
*/

/* --------------------------------------------- */
/* ----------------- INCLUDES ------------------ */
/* --------------------------------------------- */


#include <iostream>
#include <cstring>
#include <math.h>
#include <list>
#include <queue>
#include <algorithm>
#include <time.h>
//#include "rand/mersenne.cpp"
#include <stdlib.h>     /* srand, rand */ // BY FELIPE
//#include <stdio.h>

using namespace std;


/* --------------------------------------------- */
/* ---------------- DEFINICOES ----------------- */
/* --------------------------------------------- */

#define NUMITERACOES 1000
#define EPS 1e-9
#define INF 1e9
#define numArestas (numVertices-1)

struct edge {
    int a, b;
    double c;

    edge () {
        c = 0.0;
    }
};

struct nodePrufer {
    int index;
    int grau;
    list<int> adj;

    void initnodePrufer (int ind, int g) {
        index=ind;
        grau=g;
    }
    bool equal (nodePrufer &no) {
        if (index == no.index)
            return true;
        return false;
    }
    nodePrufer () {
        /*empty*/
    }

    nodePrufer (int ind, int g) {
        initnodePrufer (ind, g);
    }
    ~nodePrufer () {
        adj.clear();
    }
};

/* --------------------------------------------- */
/* ------------- VARIAVEIS GLOBAIS ------------- */
/* --------------------------------------------- */

//TRandomMersenne rg (10);
int numObjetivos = 2; //numero de objetivos do problema
int numVertices = 0; //numero de vertices da instancia
double alfa = 0.0; //variavel utilizada para definir o tamanho da lista restrita de candidatos
double ** lambda; //matriz com tantas linhas quantas forem as interacoes do grasp
//cada linha da matriz contem um vetor de escalarizacao com numObjetivos elementos
double *** custos; //matrix que armazena a instancia. custos[a][b][c] == custo do objetivo a, da aresta que liga b e c.
/*
clock_t tbusca1 = 0;
clock_t tbusca2 = 0;
clock_t tbusca3 = 0;
clock_t tbusca4 = 0;
clock_t tbusca5 = 0;
clock_t tbusca6 = 0;
*/
/* --------------------------------------------- */
/* ---- PROTOTIPOS DE FUNCOES DE COMPARACAO ---- */
/* --------------------------------------------- */

inline bool compEdge(edge a, edge b);
inline bool compNodePrufer(nodePrufer * a, nodePrufer * b);


/* --------------------------------------------- */
/* ------------ DEFINICAO DE CLASSES ----------- */
/* --------------------------------------------- */


class UnionFind {
    private:
        short * pai;
        short * altura;

        short getClass(short a) {
            if (a != pai[a])
                pai[a] = getClass(pai[a]);
            return pai[a];
        }

    public:
        UnionFind() {
            pai = new short [numVertices];
            altura = new short [numVertices];
            clear();
        }
        ~UnionFind() {
            delete [] pai;
            delete [] altura;
        }
        bool sameClass(int a, int b) {
            return getClass(a) == getClass(b);
        }
        void unionClass(int a, int b) {
            if (a != b) {
                int cl[2] = {getClass(a),getClass(b)};
                if (cl[0] != cl[1]) {
                    if (altura[cl[0]] > altura[cl[1]])
                        pai[cl[1]] = cl[0];
                    else {
                        pai[cl[0]] = cl[1];
                        if (altura[cl[0]] == altura[cl[1]]) altura[cl[1]]++;
                    }
                }
            }
        }
        void clear() {
            memset(altura,0,sizeof(short)*numVertices);
            for (short i=0;i<numVertices;i++) {
                pai[i] = i;
            }
        }
};


class Solucao {
    public:
        bool calc;
        double * f;
        int ** arestas;
        UnionFind uf;

    public:
        Solucao () {
            calc = false;
            f = new double [numObjetivos];
            for (int i = 0; i < numObjetivos; i++)
                f[i] = 0.0;

            arestas = new int *[numArestas];
            for (int i = 0; i < numArestas; i++)
                arestas[i] = new int [2];
        }

        ~Solucao() {
            if (f != NULL) {
                delete [] f;
                f = NULL;

                for (int i = 0; i < numArestas; i++) 
                    delete [] arestas[i];
                delete [] arestas;
            }	
        }

        void calcFit () {
            for (int i=0; i<numObjetivos; i++)
                f[i]=0.0;
            for (int i=0; i<numArestas; i++) {
                for (int j=0; j<numObjetivos; j++)
                    f[j]+=custos[j][arestas[i][0]][arestas[i][1]];
            }
            calc = true;
        }

        /* TODO esse metodo so deve ser usado quando a solucao ja existir mesmo,
         * ou seja, vetor de arestas sem lixo.*/
        void calcUF () {
            uf.clear();
            for (int i=0; i<numArestas; i++) {
                uf.unionClass(arestas[i][0], arestas[i][1]);
            }
        }

        bool confereArestas() {
            uf.clear();
            int numUsados = 0;
            bool usados[numVertices];
            for (int i=0; i<numVertices;i++)
                usados[i]=false;
            for (int i=0; i<numArestas; i++) {
                if (uf.sameClass(arestas[i][0],arestas[i][1])) return false;
                uf.unionClass(arestas[i][0],arestas[i][1]);
                for (int j=0; j<2; j++) {
                    if (!usados[arestas[i][j]]) {
                        usados[arestas[i][j]] = true;
                        numUsados++;
                    }
                }
            }
            if (numUsados != numVertices) return false;
            return true;
        }

        bool confereObjetivos() {
            double v[2] = {0.0, 0.0};
            for (int i=0; i<numArestas; i++) {
                v[0] += custos [0][arestas[i][0]][arestas[i][1]];
                v[1] += custos [1][arestas[i][0]][arestas[i][1]];
            }
            if (fabs(v[0]-f[0]) > EPS || fabs(v[1]-f[1]) > EPS) return false;
            return true;
        }

        bool operator>> (Solucao &b) {
            double diff [numObjetivos];
            for (int i=0; i<numObjetivos; i++) {
                diff[i] = b.f[i] - this->f[i];
            }
            //TODO generalizar para n objetivos
            if (diff[0] > EPS && (diff[1] > EPS || fabs(diff[1]) < EPS))
                return true;
            if ((diff[0] > EPS || fabs(diff[0]) < EPS) && diff[1] > EPS)
                return true;
            return false;
        }

        bool operator==(Solucao &s) {
            if (fabs(s.f[0]-this->f[0]) < EPS && fabs(s.f[1]-this->f[1]) < EPS)
                return true;
            return false;
        }

        const Solucao &operator=(Solucao &s) {
            this->calc = s.calc;
            for (int i = 0; i < numObjetivos; i++) {
                this->f[i] = s.f[i];
            }
            for (int i = 0; i < numArestas; i++) {
                for (int j = 0; j < 2; j++) {
                    this->arestas[i][j] = s.arestas[i][j];
                }
            }
            return *this;
        }

        void print () {
            cout << endl << "printing solution" << endl;
            for (int i = 0; i < numArestas; i++) {
                cout << '(' << arestas[i][0] << ", " << arestas[i][1] << ')';
            }
            cout << endl;
        }

};


class ParetoSet {
    public:
        list <Solucao *> lpareto;
        int lsize;
    public:
        ParetoSet () {
            lsize = 0;
        }
        ~ParetoSet () {
            /*TOSEE
              list <Solucao *>::iterator it = lpareto.begin(), it2;
              while (it != lpareto.end()) {
              it2 = it;
              it++;
              delete (*it2);
              }
              */
            lpareto.clear();
            lsize = 0;
        }
        bool addSolucao (Solucao * s) {
            /*
               print();
               cout << endl;
               cout << "confereParetoSet: " << confereParetoSet () << endl;
               */	
            if (lsize == 0) {
                Solucao * s1 = new Solucao();
                *s1 = *s;
                lpareto.push_back(s1);
                //cout << "solucao aceita" << endl;
                lsize++;
                return true;
            }
            else {
                list<Solucao *>::iterator it=lpareto.begin();

                int numitr = lsize;
                for (int i=0; i<numitr; i++) {
                    if ((*s)>>(*(*it))) {
                        //delete (*it);
                        it = lpareto.erase (it);
                        lsize--;
                    }
                    else it++;
                }
                bool nondom = true;
                it=lpareto.begin();
                for (int i=0; i<lsize; i++) {
                    if ((*(*it))>>(*s) || (*s)==(*(*it))) {
                        nondom = false;
                        break;
                    }
                    else it++;
                }
                if (nondom) {
                    //	cout << "nao dom" << endl;
                    Solucao * s1 = new Solucao();
                    //	cout << "alocado!" << endl;
                    *s1 = *s;
                    //	cout << "adding: " << s->f[0] << '\t' << s->f[1] << endl;
                    lpareto.push_back (s1);
                    //	cout << "add ok" << s->f[0] << '\t' << s->f[1] << endl;
                    lsize++;
                    //	cout << "added" << endl;
                    //	cout << "solucao aceita" << endl;
                    return true;
                }
                else {
                    //	cout << "solucao nao aceita" << endl;
                    return false;
                }
            }
        }

        void printFile (FILE *f) {
            list<Solucao *>::iterator it=lpareto.begin();
            while (it!=lpareto.end()) {
                fprintf(f,"%.10lf %.10lf\n",(*it)->f[0],(*it)->f[1]);
                it++;
            }
        }

        void print () {
            cout << "printing approxSet..." << endl;
            list<Solucao *>::iterator it=lpareto.begin();
            while (it!=lpareto.end()) {
                cout << (*it)->f[0] << '\t' << (*it)->f[1] << endl;
                it++;
            }
            cout << endl;
        }

        bool confereLsize () {
            if (lsize!=lpareto.size())
                return false;
            return true;
        }

        //0 == ok
        //1 == erro no tamanho
        //2 == erro nas solucoes
        //3 == erro no tamanho e nas solucoes
        int confereParetoSet () {
            bool lsizeok;
            if (lsize!=lpareto.size())
                lsizeok=false;
            else
                lsizeok=true;
            bool solucoesok = true;
            for (list<Solucao *>::iterator it=lpareto.begin(); it!=lpareto.end(); it++) {
                if (!(*it)->confereArestas()) {
                    solucoesok=false;
                    break;
                }
            }
            int retvalue;
            if (solucoesok && lsizeok)
                retvalue=0;
            else if (solucoesok && !lsizeok)
                retvalue=1;
            else if (!solucoesok && lsizeok)
                retvalue=2;
            else retvalue=3;
            return retvalue;
        }

};

class rmcKruskal {
    private:
        edge * edges;
        UnionFind uf;
        int nEdges;

    public:
        rmcKruskal() {
            nEdges = 0;
            edges = new edge [(numVertices*(numVertices-1))/2];
            //teremos as arestas: 1-0 / 2-0, 2-1, 3-0, 3-1, 3-2... ou seja: a > b
            for (int i=0; i<numVertices; i++) {
                for (int j=0; j<i; j++) {
                    edges[nEdges].a = i;
                    edges[nEdges].b = j;
                    nEdges++;
                }
            }
        }

        ~rmcKruskal () {
            delete [] edges;
        }

        void executar(Solucao& s, int itr) {

            for (int i=0; i<numObjetivos; i++) {
                s.f[i] = 0.0;
            }
            uf.clear();
            for (int i=0; i<nEdges; i++) {
                edges[i].c = 0.0;
                for (int j=0; j<numObjetivos; j++)
                    edges[i].c += lambda[itr][j]*custos[j][edges[i].a][edges[i].b];
            }

            // ordena as arestas do grafo
            stable_sort(edges,edges+nEdges,compEdge);

            // coloca numArestas arestas do grafo sem formar ciclo
            int cont = 0, ed = 0;
            while (cont<numArestas) {
                // monta uma lista restrita de candidatos com tamanho alfa% do total de arestas.
                int tamRCL=max( (int)(alfa*(double)nEdges), 1);
                int edgeRCL=min(nEdges-1, ed+tamRCL-1);
                // anda ate a proxima aresta que pode ser inserida
                while (uf.sameClass(edges[ed].a,edges[ed].b)) ed++;

                // escolhe aleatoriamente uma aresta da lista ate que encontre uma que nao forma ciclo
                // TODO PAY ATTENTION TO WHILE ED++ AND TIME OVERHEAD!!!!
                int esc;
                if (ed > edgeRCL)
                    esc = ed;
                else
                    esc = rand()%(edgeRCL - ed + 1) + ed; // rg.IRandom(ed,edgeRCL);

                while (uf.sameClass(edges[esc].a,edges[esc].b)){
                    esc = rand()%(edgeRCL - ed + 1) + ed; // rg.IRandom(ed,edgeRCL);
                 //   cout<<"kfjkfjkrjf"<<endl;
                }
                // coloca a aresta escolhida da RCL na solucao
                s.arestas[cont][0] = edges[esc].a;
                s.arestas[cont][1] = edges[esc].b;
                s.f[0] = s.f[0]+custos[0][s.arestas[cont][0]][s.arestas[cont][1]];
                s.f[1] = s.f[1]+custos[1][s.arestas[cont][0]][s.arestas[cont][1]];
                uf.unionClass(edges[esc].a, edges[esc].b);
                cont++;
            }

            //if (!s.confereArestas ()) {
              //  cout << "rmcKruskal->executar: arestas nao conferem!!!!!" << endl;
                //exit (1);
            //}

            //else {
                /*	
                    cout << "arvore gerada com krus->exec - custo: " << s.f[0] << '\t' << s.f[1] << endl;
                    for (int i = 0; i < numArestas; i++) {
                    cout << s.arestas[i][0] << '\t' << s.arestas[i][1] << endl;
                    }
                    */	
            //}

        }
};


class Prufer {

    public:
        int * b;
        int * p;
        bool alocada;

    public:
        void alocar () {
            b = new int [numVertices];
            p = new int [numVertices-2];
            alocada = true;
        }

        void desalocar () {
            if (b != NULL) {
                delete [] b;
                b = NULL;
            }
            if (p != NULL) {
                delete [] p;
                p = NULL;
            }
            alocada = false;	
        }

        void codificar (Solucao& s, struct nodePrufer * nodevec2) {

            //s.print();

            list <nodePrufer *> lvertex;
            struct nodePrufer * nodevec = new struct nodePrufer [numVertices];
            for (int i=0; i<numVertices;i++) {
                nodevec[i].initnodePrufer (i, 0);
            }
            for (int i=0; i<numArestas; i++) {
                // assert (s.arestas[i][0] < numVertices && s.arestas[i][0] > -1 &&
                //         s.arestas[i][1] < numVertices && s.arestas[i][1] > -1);
                nodevec[s.arestas[i][0]].grau++;
                (nodevec[s.arestas[i][0]].adj).push_back(s.arestas[i][1]);
                nodevec[s.arestas[i][1]].grau++;
                (nodevec[s.arestas[i][1]].adj).push_back(s.arestas[i][0]);
            }

            for (int i = 0; i < numVertices; i++) {
                nodevec2[i].index = nodevec[i].index;
                nodevec2[i].grau = nodevec[i].grau;
                nodevec2[i].adj = nodevec[i].adj;

            }
/*
            for (int pr = 0; pr < numVertices; pr++) {
                if (nodevec2[pr].index != pr) {
                    cout << "nodevec2 fora de ordem!!!" << endl;
                    break;
                }
                cout << "pr: " << pr << " grau: " << nodevec2[pr].grau << endl;
                for (list<int>::iterator iterat = nodevec2[pr].adj.begin(); iterat != nodevec2[pr].adj.end(); iterat++) {
                    cout << *iterat << ' ';
                }
                cout << endl;
            }
*/
            for (int i=0; i<numVertices; i++) {
                lvertex.push_back(&nodevec[i]);
            }

            lvertex.sort (compNodePrufer);

            list <nodePrufer *> toreinsert;
            list <nodePrufer *>::iterator it;
            list <int>::iterator it2;
            int lvertexsize = numVertices;
            int bpctr = 0;
            while (lvertexsize > 2) {
                /*inicializa b e p*/
                it = lvertex.begin();
                b[bpctr] = (*it)->index;
                it2 = ((*it)->adj).begin();
                p[bpctr] = *it2;

                /*remove o vert atual permanentemente da lista*/
                lvertex.erase(it);
                lvertexsize--;

                /*atualiza info no nodePrufer do vert adj*/
                /*remove o vert adj p reinsercao*/
                int itctr = 0;
                for (it=lvertex.begin(); it!=lvertex.end(); it++) {
                    if ((*it)->index == p[bpctr]) {
                        (*it)->grau--;
                        ((*it)->adj).remove(b[bpctr]);
                        toreinsert.push_back(*it);
                        it = lvertex.erase(it);
                        break;
                    }
                }
                /*reinsere o vertice ordenadamente*/
                lvertex.merge (toreinsert, compNodePrufer);

                /*atualiza a posicao de b e p*/
                bpctr++;
            }
            it = lvertex.begin();
            b[bpctr] = (*it)->index;
            bpctr++;
            it++;
            b[bpctr] = (*it)->index;


                
                
            //TODO PAY ATTENTION TO THIS DESTRUCTION!!!!
            lvertex.clear();
            //delete [] nodevec;

            //cout << "codificacao feita:" << endl;

            //cout << "b: ";
            //for (int i = 0; i < numVertices; i++)
            //cout << b[i] << "  ";
            //cout << endl;

            //cout << "p: ";
            //for (int i = 0; i < numVertices-2; i++)
            //cout << p[i] << "  ";
            //cout << endl;

            //cout << "saindo de codifica" << endl;
        }

        const Prufer &operator= (Prufer& pr) {
            this->alocada = pr.alocada;
            for (int i = 0; i < numVertices; i++) {
                this->b[i] = pr.b[i];
            }
            for (int i = 0; i < numVertices-2; i++) {
                this->p[i] = pr.p[i];
            }
            //memcpy (b, pr.b, sizeof(int)*numVertices);
            //memcpy (p, pr.p, sizeof(int)*numVertices-2);
            return *this;
        }

        Prufer () {
            alocar ();
        }

        ~Prufer () {
            desalocar ();
        }
};

/* --------------------------------------------- */
/* ------ PROTOTIPOS DE FUNCOES DO GRASP ------- */
/* --------------------------------------------- */

void leInstancia (const char * arqInst); //inicializa a matriz de custos pela leitura da instancia e faz outras verificacoes simples.
void geraVetoresEscalarizacao (double ** lambda); //ver se numObjetivos só vai poder ser 2, ou se pode variar
void buscaLocal (Solucao& s, int itr, ParetoSet * ps);

/* --------------------------------------------- */
/* ------------------- MAIN -------------------- */
/* --------------------------------------------- */



int main (int argc, const char * argv[]) { //parametros: instancia, "semente aleatoria, "  numero de vertices, num de objetivos, alfa

    srand (time(NULL)); // byFELIPE 

    // verifica numero de parametros
    if (argc != 8) {
        cout << "Parameter error. Usage: " << argv[0] << " (instance file) (random seed) (number of nodes) (number of objectives) (alfa) (approx set file) (log file)" << endl;
        exit (1);
    }

    clock_t tempoAntes, tempoDepois;

    //clock_t krusexe, addSol, busca;
    //clock_t aux1, aux2;
    tempoAntes = clock();


    //inicializa variaveis globais
    //rg.RandomInit(std::atoi(argv[2]));
    numVertices = atoi (argv[3]);
    numObjetivos = atoi (argv[4]);
    alfa = atof (argv[5]);

    custos = new double ** [numObjetivos];
    for (int i = 0; i < numObjetivos; i++) {
        custos [i] = new double *[numVertices];
        for (int j = 0; j < numVertices; j++)
            custos[i][j] = new double [numVertices];
    }

    //inicializa Conjunto de Aproximacao e rmcKruskal
    ParetoSet * conjAprox = new ParetoSet;
    rmcKruskal * krus = new rmcKruskal;

    //le instancia
    leInstancia (argv[1]);


    //cria vetores de escalarizacao pre-definidos para cada iteracao
    lambda = new double *[NUMITERACOES];
    for (int i = 0; i < NUMITERACOES; i++)
        lambda[i] = new double [numObjetivos];

    geraVetoresEscalarizacao (lambda);

    //cria solucao corrente, que ficara sendo manipulada
    Solucao * novaSolucao = new Solucao;


    //inicia laco...
    for (int i = 0; i < NUMITERACOES; i++) {
        //gera nova solucao com base no rmcKruskal
        //aux1=clock();
        //cout << "i: " << i << endl;
        krus->executar(*novaSolucao, i);

        //aux2=clock();
        //if (i != 0)
        //    krusexe += aux2-aux1;
        //else
        //   krusexe = 0;
        //atualiza conjunto de aproximacao com nova solucao
        //aux1=clock();
        bool resp = conjAprox->addSolucao(novaSolucao);
        //aux2=clock();
        //if (i != 0)
        //    addSol += aux2-aux1;
        //else
        //    addSol = 0;

        //conjAprox->print();
        //realiza busca local drop and add
        //aux1 = clock();
        buscaLocal (*novaSolucao, i, conjAprox);
        //aux2=clock();
        //if (i != 0)
        //    busca += aux2-aux1;
        //else
        //    busca = 0;

    }

    //cout << "laco terminado" << endl;
    FILE * approxfile = fopen (argv[6], "w");
    conjAprox->printFile (approxfile);
    fclose(approxfile);

    tempoDepois = clock();
    cout << "Tempo(s) = " << (double) (tempoDepois-tempoAntes) / CLOCKS_PER_SEC << endl;
    //cout << "krusexe: " << (double) krusexe / CLOCKS_PER_SEC  << endl;
    //cout << "addSol: " << (double) addSol / CLOCKS_PER_SEC << endl;
    //cout << "busca: " << (double) busca / CLOCKS_PER_SEC << endl;
    //cout << "tbusca1: " << (double) tbusca1 / CLOCKS_PER_SEC << endl;
    //cout << "tbusca2: " << (double) tbusca2 / CLOCKS_PER_SEC << endl;
    //cout << "tbusca3: " << (double) tbusca3 / CLOCKS_PER_SEC << endl;
    //cout << "tbusca4: " << (double) tbusca4 / CLOCKS_PER_SEC << endl;
    //cout << "tbusca5: " << (double) tbusca5 / CLOCKS_PER_SEC << endl;
    //cout << "tbusca6: " << (double) tbusca6 / CLOCKS_PER_SEC << endl;

    FILE * logfile = fopen (argv[7], "a");
    fprintf (logfile, "%s%d\t%lf\n", argv[1], conjAprox->lsize, (double) (tempoDepois-tempoAntes) / CLOCKS_PER_SEC);
    fclose(logfile);

    //libera memoria alocada


    for (int i = 0; i < numObjetivos; i++) {
        for (int j = 0; j < numVertices; j++)
            delete [] custos[i][j];
        delete [] custos[i];
    }

    delete [] custos;

    for (int i = 0; i < NUMITERACOES; i++)
        delete [] lambda[i];

    delete [] lambda;

    delete novaSolucao;
    delete conjAprox;
    delete krus;

    return 0;
}


void leInstancia (const char * arqInst) { //configurada para 2 objetivos...

    //cout << "1. Iniciando leitura dos custos" << endl;

    FILE *file = fopen(arqInst,"r");
    int k,l,nv;

    fscanf(file,"%d",&nv);
    if (nv != numVertices) {
        cout << "Inconsistencia entre o numero de vertices declarado no parametro e no arquivo da instancia. Saindo..." << endl;
        exit(0);
    } 
    // // le o lixo
    // char tmp[200];
    // for (int i=0;i<3;i++)
    //     fscanf(file,"%s",tmp);

    for (int i=0;i<numVertices-1;i++) {
        for (int j=i+1;j<numVertices;j++) {
            if (numVertices < 1000)
                fscanf(file,"%d %d %lf %lf",&k,&l,&custos[0][i][j],&custos[1][i][j]);
            else fscanf(file,"%lf %lf",&custos[0][i][j],&custos[1][i][j]);
            custos[0][j][i] = custos[0][i][j];
            custos[1][j][i] = custos[1][i][j];
        }
    }
    fclose(file);
    /*
       cout << "1. Leitura dos custos terminada" << endl;
       cout << "matriz de custos: " << endl;
       for (int i=0;i<numVertices-1;i++) {
       for (int j=i+1;j<numVertices;j++) {
       cout << i << '\t' << j << '\t';
       for (int k = 0; k < numObjetivos; k++)
       cout << custos[k][i][j] << '\t';
       cout << endl;
       }
       }
       */
}



void geraVetoresEscalarizacao (double ** lambda) {

    if (numObjetivos == 2) {

        double v = 0.0;
        double fator = 1.0;

        for (int i = 0; i < NUMITERACOES; i++) {
            lambda [i][0] = v/(NUMITERACOES-1.0);
            lambda [i][1] = 1.0 - lambda [i][0];
            v += fator;
        }
    }

    else
        cout << "(ERRO!!!) geraVetoresEscalarizacao: funcao nao definida para mais de 2 objetivos." << endl;

    /*
       cout << "2. geraVetoresEscalarizacao terminado!" << endl;
       for (int i = 0; i < NUMITERACOES; i++) {
       for (int j = 0; j < numObjetivos; j++) {
       cout << lambda [i][j] << '\t';
       }
       cout << endl;
       }
       cout << endl;
       */
}


void buscaLocal (Solucao& s, int itr, ParetoSet * ps) {

    //clock_t tempoBTAntes;
    //clock_t tempoBTDepois;
    //tempoBTAntes = clock();
    //clock_t tempoantes;
    //clock_t tempodps;

    //tempoantes = clock();
    
    int vertbp [numVertices]; //se vertbp = 1: esta na arvore do b, se vertbp = 0, esta na arvore do p

    /*codificar numero de prufer*/
    /*inicializar t e testrela*/
    Prufer * psol = new Prufer;
    nodePrufer * nodevec = new nodePrufer [numVertices];
    queue <int> arvb;
    list <int>::iterator itrb;
    psol->codificar (s, nodevec);
    double cost_s;

    int rem [2];
    int add [2];

    Prufer * ptestrela = new Prufer;
    *ptestrela = *psol;
    double cost_testrela;

    bool improved = true;

    //tempodps = clock();
    //tbusca1 += tempodps-tempoantes;

    while (improved) {

        //s.print();
        //tempoantes = clock();

        cost_s = lambda[itr][0]*(s.f[0]) + lambda[itr][1]*(s.f[1]);
        cost_testrela = cost_s;

        for (int j=0; j<numVertices-2; j++) {

            //tempoantes = clock();
            
            int e[2];
            int e_index;
            e[0] = psol->b[j];
            e[1] = psol->p[j];

            //cout << "b[j]: " << psol->b[j] << endl;
            //cout << "p[j]: " << psol->p[j] << endl;
            //cria o uf de s para conter a arvore s a menos da aresta (psol->b[j], psol->p[j])
            //s.uf.clear();
            for (int i=0; i<numArestas; i++) {
                if (s.arestas[i][0]==e[0] && s.arestas[i][1]==e[1]) {
                    e_index = i;
                    break;
                }
                else if  (s.arestas[i][0]==e[1] && s.arestas[i][1]==e[0]) {
                    swap (e[0], e[1]);
                    e_index = i;
                    break;
                }
                //else
                //    s.uf.unionClass(s.arestas[i][0], s.arestas[i][1]);
            }
            
            //tempodps = clock();
            //tbusca2 += tempodps-tempoantes;

            //tempoantes = clock();
            
            //for (int i = 0; i < numVertices; i++) {
            //    vertbp[i] = 0;
            //}
            //vertbp[psol->b[j]]=1;
            //for (int i = 0; i < numVertices; i++) {
            //    if (s.uf.sameClass (i, psol->b[j])) {
            //        vertbp[i]=1;
            //    }
            //}
            
            if (nodevec[psol->b[j]].grau == 1) {
                
                //if (j == 4 && itr == 0) {
                    //cout << "entrei no if 1" << endl;
                    //cout << "b[j]: " << psol->b[j] <<endl;
                    //cout << "p[j]: " << psol->p[j] << endl;
               /*     for (int pr = 0; pr < numVertices; pr++) {
                        if (nodevec[pr].index != pr) {
                            cout << "nodevec fora de ordem!!!" << endl;
                            break;
                        }
                        cout << "pr: " << pr << " grau: " << nodevec[pr].grau << endl;
                        for (list<int>::iterator iterat = nodevec[pr].adj.begin(); iterat != nodevec[pr].adj.end(); iterat++) {
                            cout << *iterat << ' ';
                        }
                        cout << endl;
                    }
                }*/
                
                for (int i = 0; i < numVertices; i++) {
                    vertbp[i] = 0;
                }
                vertbp[psol->b[j]] = 1;
            }
            else if (nodevec[psol->p[j]].grau == 1) {
                
                //if (j == 4 && itr == 0)
                //    cout << "entrei no if 2" << endl;
                for (int i = 0; i < numVertices; i++) {
                    vertbp[i] = 1;
                }
                vertbp[psol->p[j]] = 0;
            }
            else {
                
                //if (j == 4 && itr == 0) {
                //    cout << "entrei no if 3" << endl;
                 /*   cout << "b[j]: " << psol->b[j] <<endl;
                    cout << "p[j]: " << psol->p[j] << endl;
                    for (int pr = 0; pr < numVertices; pr++) {
                        if (nodevec[pr].index != pr) {
                            cout << "nodevec fora de ordem!!!" << endl;
                            break;
                        }
                        cout << "pr: " << pr << " grau: " << nodevec[pr].grau << endl;
                        for (list<int>::iterator iterat = nodevec[pr].adj.begin(); iterat != nodevec[pr].adj.end(); iterat++) {
                            cout << *iterat << ' ';
                        }
                        cout << endl;
                    }
                }
                */
                for (int i = 0; i < numVertices; i++) {
                    vertbp [i] = 0;
                }
                vertbp[psol->b[j]] = 1;
                for (itrb = nodevec[psol->b[j]].adj.begin(); itrb != nodevec[psol->b[j]].adj.end(); itrb++) {
                    if (*itrb != psol->p[j])
                        arvb.push(*itrb);
                }
                while (!arvb.empty()) {
                    int bind = arvb.front();
                    arvb.pop();
                    if (vertbp[bind] == 0) {
                        vertbp[bind] = 1;
                        itrb = nodevec[bind].adj.begin();
                        while (itrb != nodevec[bind].adj.end()) {
                            arvb.push (*itrb);
                            itrb++;
                        }
                    }
                    //else {}
                }
                
            }
            /*
            cout << "vertbp: " << endl;
            for (int i = 0; i < numVertices; i++) {
                cout << vertbp[i] << ' ';
            }
            cout << endl;
            */
            //tempodps = clock();
            //tbusca3 += tempodps-tempoantes;


            //cout << "unionfind defined para retirar a aresta: " << e[0] << '-' << e[1] <<endl;

            //tenta adicionar todas as arestas c um vertice da subarvore de psol->p[j] e psol->b[j]
            //TODO VER ATUALIZACAO DE COST_S

            //tempoantes = clock();

            for (int i=0; i<numVertices; i++) {
                if (i!=psol->p[j]) {
                    if (vertbp[i]==0) {
                        double newcost = cost_s -
                            (lambda[itr][0]*custos[0][e[0]][e[1]]+lambda[itr][1]*custos[1][e[0]][e[1]]) +
                            (lambda[itr][0]*custos[0][psol->b[j]][i]+lambda[itr][1]*custos[1][psol->b[j]][i]);
                        if (newcost < cost_testrela) {

                            rem[0] = e[0];
                            rem[1] = e[1];
                            add[0] = psol->b[j];
                            add[1] = i;

                            cost_testrela = newcost;

                            //atualiza prufer de testrela
                            int ctr = 0;
                            for (int l=0; l<numVertices; l++) {
                                if (vertbp[psol->b[l]]==1 && l!=j) {
                                    ptestrela->b[ctr] = psol->b[l];
                                    ptestrela->p[ctr] = psol->p[l];
                                    ctr++;
                                }
                            }
                            ptestrela->b[ctr] = psol->b[j];
                            ptestrela->p[ctr] = i;
                            ctr++;
                            for (int l=0; l<numVertices; l++) {
                                if (vertbp[psol->b[l]]==0) {
                                    ptestrela->b[ctr] = psol->b[l];
                                    if (ctr < numVertices-2)
                                        ptestrela->p[ctr] = psol->p[l];
                                    ctr++;
                                }
                            }

                            /*
                               UnionFind uf;
                               uf.clear();
                               bool conftestr = true;
                               int numUsados = 0;
                               bool usados[numVertices];
                               for (int u=0; u<numVertices;u++)
                               usados[u]=false;
                               for (int u=0; u<numVertices-1; u++) {
                               if (u < numVertices-2) {
                               if (uf.sameClass(ptestrela->b[u],ptestrela->p[u])) {
                               conftestr = false;
                               break;
                               }
                               uf.unionClass(ptestrela->b[u],ptestrela->p[u]);
                               if (!usados[ptestrela->b[u]]) {
                               usados[ptestrela->b[u]] = true;
                               numUsados++;
                               }
                               if (!usados[ptestrela->p[u]]) {
                               usados[ptestrela->p[u]] = true;
                               numUsados++;
                               }
                               }
                               else {
                               if (uf.sameClass(ptestrela->b[u],ptestrela->b[u+1])) {
                               conftestr = false;
                               break;
                               }
                               uf.unionClass(ptestrela->b[u],ptestrela->b[u+1]);
                               if (!usados[ptestrela->b[u]]) {
                               usados[ptestrela->b[u]] = true;
                               numUsados++;
                               }
                               if (!usados[ptestrela->b[u+1]]) {
                               usados[ptestrela->b[u+1]] = true;
                               numUsados++;
                               }
                               }
                               }
                               if (numUsados != numVertices) conftestr = false;
                               if (conftestr) {}
                            //cout << "confereTestrela ok!!" << endl;
                            else
                            cout << "t estrela nao confere!!" << endl;
                            */
                        }
                    }
                }
            }

            //tempodps = clock();
            //tbusca4 += tempodps-tempoantes;
        }

        //tempoantes = clock();
        if (cost_testrela < cost_s) {

            nodevec [rem[0]].grau -= 1;
            for (list<int>::iterator iter = nodevec [rem[0]].adj.begin(); iter != nodevec[rem[0]].adj.end(); iter++) {
                if (*iter == rem[1]) {
                    nodevec[rem[0]].adj.erase (iter);
                    break;
                }
            }
            nodevec [rem[1]].grau -= 1;
            for (list<int>::iterator iter = nodevec [rem[1]].adj.begin(); iter != nodevec[rem[1]].adj.end(); iter++) {
                if (*iter == rem[0]) {
                    nodevec[rem[1]].adj.erase (iter);
                    break;
                }
            }
            nodevec [add[0]].grau +=1;
            nodevec [add[0]].adj.push_back(add[1]);
            nodevec [add[1]].grau += 1;
            nodevec [add[1]].adj.push_back(add[0]);
            
            // s = testrela e psol eh atualizado para conter ptestrela
            *psol = *ptestrela;
            for (int i=0; i<numVertices-2; i++) {
                if (psol->b[i] > psol->p[i]) {
                    s.arestas[i][0] = psol->b[i];
                    s.arestas[i][1] = psol->p[i];
                }
                else {
                    s.arestas[i][0] = psol->p[i];
                    s.arestas[i][1] = psol->b[i];
                }
            }
            if (psol->b[numVertices-2] > psol->b[numVertices-1]) {
                s.arestas[numArestas-1][0] = psol->b[numVertices-2];
                s.arestas[numArestas-1][1] = psol->b[numVertices-1];
            }
            else {
                s.arestas[numArestas-1][0] = psol->b[numVertices-1];
                s.arestas[numArestas-1][1] = psol->b[numVertices-2];
            }
            s.calcFit();
            //if (!s.confereArestas())
            //    cout << "confere arestas de s errado" << endl;
            //else
            //cout << "confere arestas de s ok" << endl;
            //o conjunto de aproximacao eh atualizado com t
            bool resp = ps->addSolucao(&s);
            //if (resp)
            //cout << "s entrou no ps!!" << endl;
            //else
            //cout << "s fora de ps!!" << endl;

        }
        else {
            //cout << "cai foi no else" << endl;
            improved = false;
        }
        //tempodps = clock();
        //tbusca5 += tempodps-tempoantes;

    }

    //tempoBTDepois = clock();
    //tbusca6 += tempoBTDepois-tempoBTAntes;
}


inline bool compEdge(edge a, edge b) {
    if (a.c < b.c) return true;
    return false;
}


    inline bool compNodePrufer(nodePrufer * a, nodePrufer * b) {
        if (a->grau < b->grau)
            return true;
        else if (a->grau==b->grau && a->index < b->index)
            return true;
        return false;
    }


#endif
