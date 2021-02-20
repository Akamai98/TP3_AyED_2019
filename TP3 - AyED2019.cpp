#include<iostream>
#include<string.h>

#define INF 5000
#define MAXNODOS 12
#define MIEMBRO 1
#define NO_MIEMBRO 0

using namespace std;

//Funciones aparte
int* dijkstra(int[][MAXNODOS],int,int,int[]);
void mostrar_ciudades(int);
void camino(int[],int,int);

//Nodo
class Nodo
{
	private:
		string palabra;
		Nodo* next;
	public:
		Nodo() { this->next = NULL; }
		Nodo(string);
		~Nodo() {};
		Nodo* get_next() { return this->next; }
		string get_palabra() { return this->palabra; }
		bool esta_vacio() { return this->get_next()==NULL; }
		void set_next(Nodo* next) { this->next = next; }
};

//Constructor de nodo
Nodo::Nodo(string palabra)
{
	this->palabra = palabra;
	this->next = NULL;
}

//Lista
class Lista
{
	private:
		Nodo* comienzo;
	public:
		Lista() { comienzo = new Nodo(); }
		~Lista() {};
		Nodo* get_comienzo() { return this->comienzo; }
		int cantidad_elementos();
		bool esta_vacia() { return this->comienzo->get_next()==NULL; }
		void add(string);
		void borrar(string);
};

//Métodos de la lista
int Lista::cantidad_elementos()
{
	int c = 0;

	Nodo* pivote = this->comienzo;
	
	while(pivote->get_next()!=NULL)
	{
		c++;

		pivote = pivote->get_next();
	}

	return c;
}

void Lista::add(string s)
{
	Nodo* nuevoNodo = new Nodo(s);
	
	nuevoNodo->set_next(this->comienzo);
	
	comienzo = nuevoNodo;
}

//Funciones aparte
void saca_precios(Lista*,int[12][12]);
void multiplicar(Lista*[12][12],string[12][12]);
bool hay_letra_repetida(string);

int precio_barato = 0;

Nodo* baratoH;

int main()
{
	int *pdist,i,j,P[MAXNODOS],s,t;

	s = 0; //Vértice de inicio -> Madrid
	t = 9; //Vértice final -> Moscú
	
	Lista *M1_lista[12][12],*Hamiltons = new Lista();

	int M_ad[12][12] = {
							INF, 150, 250, 210,  340, INF, 300, INF, INF,  INF, INF, INF,
							INF, INF, 220, 200,  350, 695, INF, INF, INF,  INF, INF, INF,
							INF, 210, INF, 350,  440, INF, 150, INF, INF,  900, 250, 320,
							INF, INF, 370, INF,   95, 100, INF, 200, INF,  INF, 320, INF,
							INF, INF, 320,  95,  INF, 150, 240, INF, INF, 1200, INF, INF,
							INF, INF, 335,  90,  140, INF, 200, 160, INF,  INF, INF, INF,
							INF, 290, 145, INF,  240, 200, INF, 120, INF,  INF, 450, 380,
							INF, INF, 200, 195,  330, 160, 110, INF,  60,  470,  80,  90,
							INF, INF, INF, INF, 1000, 570, 500,  10, INF,  220,  60,  35,
							INF, INF, INF, INF,  INF, INF, INF, INF, INF,  INF, INF, INF,
							INF, INF, 250, INF,  INF, 180, 150,  90,  50,  INF, INF,  15,
							INF, INF, 300, INF,  INF, INF, INF,  90,  20,  180,  10, INF
						};
						
	string M1[12][12] = {
						    "0", "12", "13", "14", "15",  "0", "17",  "0",  "0",  "0",  "0",  "0",
							"0",  "0", "23", "24", "25", "26",  "0",  "0",  "0",  "0",  "0",  "0",
							"0", "32",  "0", "34", "35",  "0", "37",  "0",  "0", "3A", "3B", "3C",
							"0",  "0", "43",  "0", "45", "46",  "0", "48",  "0",  "0", "4B",  "0",
							"0",  "0", "53", "54",  "0", "56", "57",  "0",  "0", "5A",  "0",  "0",
							"0",  "0", "63", "64", "65",  "0", "67", "68",  "0",  "0",  "0",  "0",
							"0", "72", "73",  "0", "75", "76",  "0", "78",  "0",  "0", "7B", "7C",
							"0",  "0", "83", "84", "85", "86", "87",  "0", "89", "8A", "8B", "8C",
							"0",  "0",  "0",  "0", "95", "96", "97", "98",  "0", "9A", "9B", "9C",
							"0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",
							"0",  "0", "B3",  "0",  "0", "B6", "B7", "B8", "B9",  "0",  "0", "BC",
							"0",  "0", "C3",  "0",  "0",  "0",  "0", "C8", "C9", "CA", "CB",  "0"
					    };

	string M2[12][12] = {
							"0", "2", "3", "4", "5", "0", "7", "0", "0", "0", "0", "0",
							"0", "0", "3", "4", "5", "6", "0", "0", "0", "0", "0", "0",
							"0", "2", "0", "4", "5", "0", "7", "0", "0", "A", "B", "C",
							"0", "0", "3", "0", "5", "6", "0", "8", "0", "0", "B", "0",
							"0", "0", "3", "4", "0", "6", "7", "0", "0", "A", "0", "0",
							"0", "0", "3", "4", "5", "0", "7", "8", "0", "0", "0", "0",
							"0", "2", "3", "0", "5", "6", "0", "8", "0", "0", "B", "C",
							"0", "0", "3", "4", "5", "6", "7", "0", "9", "A", "B", "C",
							"0", "0", "0", "0", "5", "6", "7", "8", "0", "A", "B", "C",
							"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0",
							"0", "0", "3", "0", "0", "6", "7", "8", "9", "0", "0", "C",
							"0", "0", "3", "0", "0", "0", "0", "8", "9", "A", "B", "0"
					    };

	//Dijkstra:
	
	pdist = dijkstra(M_ad,s,t,P);

	if(pdist[t]!=INF)
	{
		cout << "EL PRECIO MINIMO DE MADRID A MOSCU ES: " << pdist[t];
	   
	    cout << endl << endl << "EL CAMINO ES ";

	    camino(P,s,t);
	}
	else cout << endl << "NO HAY CAMINO POSIBLE";
	
	cout << endl << endl;

	for(int i=0; i<12; i++)
		for(int j=0; j<12; j++)
		{
			M1_lista[i][j] = new Lista();

			if(M1[i][j].compare("0")!=0) M1_lista[i][j]->add(M1[i][j]);
			else M1_lista[i][j]->add("0");
		}

	//Camino Hamiltoniano más barato:
	
	cout << endl << endl << "------------------------------------------------------------------------------------------------------------------------" << endl;
	
	Nodo* pivoteH;
	
	int PBH;
	
	bool primera_vez = true;

	for(int i=0; i<10; i++)
	{
		multiplicar(M1_lista,M2);
		
		if(i==7)
		{
			for(int j=0; j<12; j++)
				for(int k=0; k<12; k++)
				{
					pivoteH = M1_lista[j][k]->get_comienzo();
					
					if(pivoteH->get_palabra().compare("0")!=0 && pivoteH->get_next()->get_palabra().compare("0")!=0 && pivoteH->get_next()!=NULL)
					{
						saca_precios(M1_lista[j][k],M_ad);
						
						if(primera_vez)
						{
							PBH = precio_barato;
							
							primera_vez = false;
						}
						else if(precio_barato<PBH) PBH = precio_barato;
					}
				}
		}
	}
	
	cout << endl << endl << "EL CAMINO MAS BARATO RECORRIENDO 10 CIUDADES CUESTA: " << PBH << endl
		 << "EL CAMINO ES ";
		 
	for(int i=0; i<baratoH->get_palabra().length(); i++)
	{
		if(baratoH->get_palabra().at(i)>=48 && baratoH->get_palabra().at(i)<=57) mostrar_ciudades(baratoH->get_palabra().at(i)-48);
		else mostrar_ciudades(baratoH->get_palabra().at(i)-55);
	}

	Nodo* pivote = M1_lista[0][9]->get_comienzo();
	
	while(pivote->get_next()!=NULL)
	{
		Hamiltons->add(pivote->get_palabra());
		
		pivote = pivote->get_next();
	}
	
	cout << endl << endl << "------------------------------------------------------------------------------------------------------------------------"
	 	 << endl << endl << "CANTIDAD DE CAMINOS HAMILTONIANOS: " << Hamiltons->cantidad_elementos() << endl << endl
		 << "EL PRECIO MAS BARATO RECORRIENDO TODAS LAS CIUDADES ES: ";

	saca_precios(Hamiltons,M_ad);

	cout << endl << endl;
	
	system("PAUSE");

	return EXIT_SUCCESS;
}

void saca_precios(Lista* Hamilton, int M_ad[12][12])
{
	Nodo *pivote = Hamilton->get_comienzo(),*nodo_barato = pivote;

	string caminoH;

	bool primera_vez = true;

	int v1,v2,precio_camino = 0;
	
	precio_barato = 0;
	
	while(pivote->get_next()!=NULL)
	{
		precio_camino = 0;
		
		if(pivote->get_palabra().compare("0")!=0) caminoH = pivote->get_palabra();
		else
		{
			pivote = pivote->get_next();
			
			continue;
		}
		
		for(int i=0; i<caminoH.length()-1; i++)
		{
			if(caminoH.at(i)>=48 && caminoH.at(i)<=57) v1 = caminoH.at(i) - 49;
			else v1 = caminoH.at(i) - 56;
			
			if(caminoH.at(i+1)>=48 && caminoH.at(i+1)<=57) v2 = caminoH.at(i+1) - 49;
			else v2 = caminoH.at(i+1) - 56;
			
			precio_camino += M_ad[v1][v2];
			
			if(precio_camino>precio_barato && !primera_vez) break;
		}
		
		if((precio_camino<precio_barato && precio_camino>0) || primera_vez)
		{
			precio_barato = precio_camino;
			
			nodo_barato = pivote;
			
			if(precio_barato==950) baratoH = pivote;
		}
		
		primera_vez = false;
		
		pivote = pivote->get_next();
	}
	
	cout << precio_barato << endl << "EL RECORRIDO ES ";
	
	for(int i=0; i<nodo_barato->get_palabra().length(); i++)
	{
		if(nodo_barato->get_palabra().at(i)>=48 && nodo_barato->get_palabra().at(i)<=57) mostrar_ciudades(nodo_barato->get_palabra().at(i)-48);
		else mostrar_ciudades(nodo_barato->get_palabra().at(i)-55);
	}
}

void multiplicar(Lista* M1_lista[12][12], string M2[12][12])
{
	string aux;

	Lista* matriz[12][12];

	Nodo* pivote;

	for(int i=0; i<12; i++)
		for(int j=0; j<12; j++)
		{
			matriz[i][j] = new Lista();
			matriz[i][j]->add("0");
		}
	
	for(int i=0; i<12; i++)
	{
		aux.clear();

		for(int j=0; j<12; j++)
		{
			if(i!=j)
			{
				aux.clear();

				for(int k=0; k<12; k++)
				{
					aux.clear();
					
					pivote = M1_lista[i][k]->get_comienzo();
						
					while(pivote->get_next()!=NULL)
					{
						aux.clear();
						
						if(pivote->get_palabra().compare("0")!=0 && M2[k][j].compare("0")!=0)
						{
							(aux.append(pivote->get_palabra())).append(M2[k][j]);
							
							if(!hay_letra_repetida(aux)) matriz[i][j]->add(aux);
						}
						else break;
	
						pivote = pivote->get_next();
					}
				}
			}
			else matriz[i][j]->add("0");
		}
	}
	
	for(int i=0; i<12; i++)
		for(int j=0; j<12; j++) M1_lista[i][j] = matriz[i][j];
}

bool hay_letra_repetida(string S)
{
	for(int i=0; i<S.length(); i++)
		for(int j=0; j<S.length(); j++)
			if(i!=j && S.at(i)==S.at(j)) return true;

	return false;
}

int* dijkstra(int C[][MAXNODOS], int s, int t, int P[])
{
	static int D[MAXNODOS];

	int S[MAXNODOS],actual,i,k,b,menordist,nuevadist;
	  
	for(i=0; i<MAXNODOS; i++)
	{
		S[i] = NO_MIEMBRO;
	    D[i] = INF;
	    P[i] = -1;
	}
	
	S[s] = MIEMBRO;
	D[s] = 0;

	actual = s;
	b = 1;
	  
	while((actual!=t)&&(b==1))
	{
		b = 0;
	  
	    menordist = INF;
	    
	    for(i=0; i<MAXNODOS; i++)
	    { 
		    if(S[i]==NO_MIEMBRO)
		    {
		        nuevadist = D[actual] + C[actual][i];
		        
		        if(nuevadist<D[i])
		        {
		        	D[i] = nuevadist;
			        P[i] = actual;

			        b = 1;
		        }
		        
		        if(D[i]<menordist)
		        {
		        	menordist = D[i];

			        k = i;
			        b = 1;
		        }
		    }
	    }

	    actual = k; 
	    
	    S[actual] = MIEMBRO;
	}

	return D;
}

void camino(int P[], int s, int t)
{
	if(t==s)
	{
		s++;
		mostrar_ciudades(s);
	}
	else
	{
	    camino(P,s,P[t]);

	    t++;

	    mostrar_ciudades(t);
	}
}

void mostrar_ciudades(int s)
{
	if(s==1) cout << "-> MADRID ";
	else if(s==2) cout << "-> BARCELONA ";
	else if(s==3) cout << "-> ROMA ";
	else if(s==4) cout << "-> PARIS ";
	else if(s==5) cout << "-> LONDRES ";
	else if(s==6) cout << "-> AMSTERDAM ";
	else if(s==7) cout << "-> ZURICH ";
	else if(s==8) cout << "-> BERLIN ";
	else if(s==9) cout << "-> VARSOVIA ";
	else if(s==10) cout << "-> MOSCU ";
	else if(s==11) cout << "-> VIENA ";
	else cout << "-> BUDAPEST ";
}
