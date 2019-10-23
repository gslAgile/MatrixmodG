#include "app.hpp"

void copia_vector(vector_o& v_dst, const vector_o& v_src)
{
	size_t i;
	if((!v_dst.empty()) && (!v_src.empty()) && (v_dst.size() == v_src.size()))
	{
		for(i=0; i< v_src.size(); i++)
			v_dst[i] = v_src[i];
	}
	else
		cout << " Falla en copia de vector." << endl;
}



int main() {

	/* Archivos de importacion de RDPG: */
	string n_mII = "./Caso_Aplicacion/mII.txt";
	string n_mIH = "./Caso_Aplicacion/mIH.txt";
	string n_mIR = "./Caso_Aplicacion/mIR.txt";
	string n_mIRe = "./Caso_Aplicacion/mIRe.txt";// = "mIRe.txt";
	string n_vMI = "./Caso_Aplicacion/vMI.txt";

	//RDPG red("Red de Petri", 6, 4);
	//RDPG *red = new RDPG("Red de Petri", 1000, 1000);
	//RDPG *red = new RDPG("Red 1", n_mII, n_mIH, n_mIR, n_mIRe, n_vMI);
	//RDPG red ("Red 1", n_mII, n_mIH, n_mIR, n_mIRe, n_vMI);
	
	monitor *red = new monitor("Red 1", n_mII, n_mIH, n_mIR, n_mIRe, n_vMI);	

 	cout << " Hola, mundo RDPG! " << endl;

	/*red->print_RDPGinfo();
	//red.print_RDPGinfo();

	size_t pos =1, vdim=10; 
	int i=0;
	matrix_o& comp = red->ref_mcomp(_mII);
	comp[1][1] = 20;
	comp[20][1] = 20;
	while((pos!=0) && (i<10))
	{
		cin >> vdim >> pos;

		red->set_vdim(vdim);
		red->set_posVP(pos);
		red->set_posVT(pos);

		red->print_mcomp(_mD);
		red->print_mcomp(_mII);
		red->print_vcomp(_vMI);
		red->print_vcomp(_vE);
		//red.print_mcomp(_mIRe);

		//red.print_mcomp(_vMI);

		
		i++;
	}*/

 	if(!red->empty())
	{

	    //usleep(100);

		thread t2([&]() {
	        while(true)
	        {
	        	red->encender_CPU1();
	        	red->apagar_CPU1();
	        }
	    });


	    thread t4([&]() {
	        while(true)
	        {
	        	red->encender_CPU2();
	        	red->apagar_CPU2();
	        }
	    });


	    thread t3([&]() {
	        while(true)
	        {
	        	red->procesar_tarea_CPU1();
	        	//sleep(2);
	        	red->finalizar_tarea_CPU1();
	        }
	    });


	    thread t5([&]() {
	        while(true)
	        {
	        	red->procesar_tarea_CPU2();
	        	//sleep(1);
	        	red->finalizar_tarea_CPU2();
	        }
	    });

	    sleep(3);

	    thread t1([&]() {
	        for(size_t i=0; i<N_TASKS; i++)
	        {
	        	red->generar_tarea();
	        }
	        red->print_vcomp(_vMA);
	    });


		int opcion = 0, op_thread = 0;
	    while( opcion < 4)
		{
		    cout << "\n--------------------------------------------------------------------------------\n";
	    	cout <<  "\t\tAPP C++ - MENU DE OPERACIONES RDPG \n";
	    	cout << "--------------------------------------------------------------------------------\n";
	        cout <<  "\n  1. Ver estado de RDPG (marcado actual).";
	        cout <<  "\n  2. Ver estado completo de RDPG (todos los componentes).";
	        cout <<  "\n  3. Ver vector Ex de transicicones sensibilizadas extendido en la RDPG.";
	        cout <<  "\n  4. Salir y finalizar hilos.";
	        cout <<  "\n  5. Salir y esperar hilos.";
	        cout <<  "\n\n   Ingrese numero de opcion: ";
	        cin >> opcion;


	        switch(opcion)
	        {
	        	case 1: 
	        			red->print_vcomp(_vMA);
	        			break;

	        	case 2:	
	        			red->print_allComp();
	        			break;

	        	case 3:	
	        			red->print_vcomp(_vEx);
	        			break;

	        	case 4:	
	        			op_thread = 1;
	        			break;

	        	case 5:
	        			op_thread = 2;
	        			break;

	        	default:
	        			cout<< "Opcion incorrecta!" << endl;
	        }
        }

        if(op_thread == 2)
		{
			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();	
		}
	}


	delete red;

	return 0;
}