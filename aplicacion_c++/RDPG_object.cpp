/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * Libreria RDPG_object.hpp/.cpp (archivo fuente - source file).
 * 
 * 
 * Esta libreria contiene las declaraciones y definiciones de la clase RDPG y sus metodos asociados. Esta clase permite
 * gestionar las RDPG como objetos. 
 * 
 * 
 * La libreria fue migrada desde codigo C del driver MatrixmodG, es el codigo replica en C++, la lubreria de C fue testeada por un conjunto de pruebas unitarias
 * e integrales mediante el framework Kernel Test Framework (KTF). Pero no se repitieron las pruebas unitarias e integrales desde el espacio usuario, en caso de
 * ser realizado se recomienda hacerlo con el framework de pruebas GTEST.
 * 
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "RDPG_object.hpp"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE VARIABLES GLOBALES PARA OBJETOS RDPG
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int RDPG::n_objects=0; /* Inicializo contador global de objetos. */

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DEFINICION/IMPLEMENTACION DE METODOS DE LIBRERIA RDPG_object.hpp
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Constructor personalizado 1: Realiza la inicializacion de todos los atributos de un objeto RDPG de acuerdo al numero de las plazas y 
 * 			   transiciones que se envian por parametro.
 *
 * @param[in]  p_name         Nombre de la RDPG.
 * @param[in]  p_places       Numero de plazas de la RDPG.
 * @param[in]  p_transitions  Numero de transiciones de la RDPG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
RDPG::RDPG(string p_name, size_t p_places, size_t p_transitions)
: mII(p_places, vector_o(p_transitions)), mIH(p_places, vector_o(p_transitions)),
  mIR(p_places, vector_o(p_transitions)), mIRe(p_places, vector_o(p_transitions)),
  mD(p_transitions, vector_o(p_transitions)),
  vMI(p_places), vMA(p_places), vMN(p_places), vE(p_transitions),
  vQ(p_places), vW(p_places), vL(p_transitions,1), vB(p_transitions,1), 
  vG(p_transitions,1), vA(p_transitions), vUDT(p_transitions), vEx(p_transitions),
  vHQCV(p_transitions), vHD(p_transitions),
  name(p_name), plazas(p_places), transiciones(p_transitions),
  posVP(0), posVT(0), vdim(MAX_VDIM), error_code(EC_NULL), shot_result(SHOT_INIT)
{
	identity_matrix(_mD);
	obj_id = (int)n_objects;
	n_objects++;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Constructor personalizado 2: Realiza la inicializacion de los atributos de un objeto RDPG de acuerdo con la lectura de los archivos *.txt de
 * 			   los componetes base de una RDPG enviados por parametro.
 *
 * @param[in]  p_name  Nombre de la RDPg.
 * @param[in]  p_mII   Nombre del archivo asociado al componete matrix_o mII de la RDPG.
 * @param[in]  p_mIH   Nombre del archivo asociado al componete matrix_o mIH de la RDPG.
 * @param[in]  p_mIR   Nombre del archivo asociado al componete matrix_o mIR de la RDPG.
 * @param[in]  p_mIRe  Nombre del archivo asociado al componete matrix_o mIRe de la RDPG.
 * @param[in]  p_vMI   Nombre del archivo asociado al componete vector_o vMI de la RDPG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
RDPG::RDPG(string p_name, string p_mII, string p_mIH, string p_mIR, string p_mIRe, string p_vMI)
: name(p_name), posVP(0), posVT(0), vdim(MAX_VDIM), error_code(EC_NULL), shot_result(SHOT_INIT)
{
	/* Determino numero de plazas y transiciones de RDPG desde archivo p_mII (su matriz de incidencia). */
	plazas = get_fileLines(p_mII);
	transiciones = get_lineElements(p_mII);

	if((plazas > 0) && (transiciones > 0))
	{
		/* Redimensiono los componentes matrix_o y vector_o de la RDPG. */
		resize_allmatrixs();
		resize_allvectors();
		import_RDPG(p_mII, p_mIH, p_mIR, p_mIRe, p_vMI);
		cpy_vector(vB, vector_o (vB.size(), 1));
		cpy_vector(vL, vector_o (vL.size(), 1));
		cpy_vector(vMA, vMI);
		identity_matrix(_mD);
		update_vG();
		update_work_components();

		if(RDPGO_DB_MSG) cout << "Se importo exitosamente RDPG: (" << name << ") desde archivos txt." << endl;
	}
	else
	{
		cout << "No se pudo importar exitosamente la RDPG: (" << name << ") desde archivos txt." << endl;
	}

	obj_id = (int)n_objects;
	n_objects++;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * METODOS GETTERS
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo retorna el numero de lineas de un archivo. 
 * 			   This method gets the file lines.
 *
 * @param[in]  p_fname  Nombre del archivo sobre el cual leer el numero de lineas.
 *
 * @return     El retorno es el numero de lineas del archivo, si tuvo exito la operacion.
 * 			   El retorno sera -EC_fileReadFail, si no tuvo exito la operacion.
 * 			   The return is file lines, if the operation was successful.
 * 			   The return will be -EC_fileReadFail, if operation no was successful.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t RDPG::get_fileLines(string p_fname)
{
	ifstream fin; 						/* Objeto para lectura de archivo. */
	string line;						/* Cadena que almacena una linea del archivo. */
	size_t i = 0;

	fin.open(p_fname.c_str());			/* Apertura de archivo. */

	if(!fin.is_open())					/* Si fallo apertura de archivo, se retorna el fallo. */
	{
		cout << "   --> ERROR: No se pudo abrir archivo ( " << p_fname << " ) para su lectura." << endl;
		return -EC_fileReadFail;
	}

	getline(fin, line);					/* Captura de primer linea de archivo. */

	while( !fin.eof() ){ 
		getline(fin, line); i++;		/* Cuento numero de lineas en archivo. */
		
		if(i > MAX_PLACES)				/* Error por exeso de plazas soportadas por APP. */
			return (size_t)0;
	}

	return i;							/* Retorno numero de lineas de archivo. */
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo retorna el numero de elementos de la primer linea de un archivo.
 * 			   This method gets the elements number of file line first.
 *
 * @param[in]  p_fname  Nombre del archivo sobre el cual realizar la operacion.
 *
 * @return     Se retorna el nummero de elementos de la primer linea del archivo, si la operacion tuvo exito.
 * 			   Se retorna -EC_fileReadFail, si la operacion no tuvo exito.
 * 			   Return the elements number of file line first, if operation was successful.
 * 			   Return -EC_fileReadFail, if operation not was successful.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t RDPG::get_lineElements(string p_fname)
{
	ifstream fin; 						/* Objeto para lectura de archivo. */
	string line;						/* Cadena que almacena una linea del archivo. */

	fin.open(p_fname.c_str());			/* Apertura de archivo. */

	if(!fin.is_open())					/* Si fallo apertura de archivo, se retorna el fallo. */
	{
		cout << "   --> ERROR: No se pudo abrir archivo ( " << p_fname << " ) para su lectura." << endl;
		return -EC_fileReadFail;
	}

	getline(fin, line);					/* Captura de primer linea de archivo. */

	istringstream dline(line);			/* Objeto para tratar string como un stream de entrada. Se inicia con dline (data line).*/
	int element;						/* Entero para almacenar elementos de matriz.*/
	size_t j=0;

	while(dline >> element){ 
		j++; 							/* Cuento numero de elementos enteros en linea. */
		if(j > MAX_TRANSITIONS)			/* Error por exeso de transiciones soportadas por APP. */
			return (size_t)0;
	}
	
	return j;							/* Retorno numero de elementos enteros en linea. */
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo retorna el numero de tokens de la plaza enviada por parametro.
 *
 * @param[in]  p_place  Numero de la plaza sobre la que se desea conocer el numero de tokens.
 *
 * @return     Se retorna el numero de tokens de la plaza enviada por parametros, si lo operacion tuvo exito.
 * 			   Se retorna -EC_datoIncorrecto, si la operacion no tuvo exito.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int RDPG::get_TokensPlace(size_t p_place)
{
	if(p_place < plazas) /* Validacion de direccion correcta en vector. */
	{
		if(RDPGO_DB_MSG) cout << "Info: Se retorna numero de tokens de plaza P" << p_place << endl;
		return vMA[p_place];
	}
	else
	{
		cout << "Error: Numero de plaza incorrecto. La plaza P" << p_place << " no existe en la RDPG: ("<< name <<")." << endl;
		error_code = -EC_datoIncorrecto;
		return -EC_datoIncorrecto;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo retorna el tamaño de un componete matriz (matrix_o) de la RDPG.
 *
 * @param[in]  p_mcomp  Identificador de componente matriz sobre el que se desea conocer el tamaño.
 *
 * @return     Se retorna el tamaño del componente matriz, si la operacion tuvo exito.
 * 			   Se retorna cero, si la operacion no tuvo exito.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t RDPG::get_mcompSize(ID_MCOMPONENT p_mcomp) const
{	
	if((p_mcomp >= ID_MC_INIT) && (p_mcomp < ID_MC_END))
	{
		if(p_mcomp != _mD)
		{
			return (size_t) (4*mII.size() + (4*mII.size()*mII[0].size()));
		}
		else if(p_mcomp == _mD)
		{
			return (size_t) (4*mD.size() + (4*mD.size()*mD.size()));
		}
	}
	else
		return 0;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo retorna el tamaño de un componete vector (vector_o) de la RDPG.
 *
 * @param[in]  p_vcomp  Identificador del componente vector sobre el que se desea conocer el tamaño.
 *
 * @return     Se retorna el tamaño del componente vector, si la operacion tuvo exito.
 * 			   Se retorna cero, si la operacion no tuvo exito.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t RDPG::get_vcompSize(ID_VCOMPONENT p_vcomp) const
{
	if((p_vcomp >= ID_VC_INIT) && (p_vcomp < ID_VC_END))
	{	
		if((p_vcomp == _vMI) || (p_vcomp == _vMA) || (p_vcomp == _vMN) || (p_vcomp == _vW) || (p_vcomp == _vQ))
		{
			return (size_t) 4*vMI.size();
		}
		else if(p_vcomp < ID_VC_END)
		{
			return (size_t) 4*vE.size();
		}
	}
	else
		return 0;

}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo retorna el tamaño bytes utilizada por el objeto RDPG.
 *
 * @return     Retorna el tamaño en bytes del objeto RDPG de acuerdo a el tamaño en bytes cada uno de sus componentes.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t RDPG::get_RDPGSize() const
{
	size_t bytes=0;

	/* Medicion de memoria sobre todos los componetes matrix_o. */
	for (int i = 0; i < ID_MC_END; i++)
	{
		bytes+= (size_t)(get_mcompSize((ID_MCOMPONENT)i));
	}

	/* Medicion de memoria sobre todos los componetes vector_o. */
	for (int i = 0; i < ID_VC_END; i++)
	{
		bytes+= (size_t)(get_vcompSize((ID_VCOMPONENT)i));
	}

	/* Medicion de memoria sobre resto de atributos de clase RDPG. */
	bytes+= sizeof(RDPG);

	return bytes;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo determina si la transicion enviada por parametro esta o no sesibilizada.
 *
 * @param[in]  p_transicion  Numero de la transicion a determinar su sensibilizado.
 *
 * @return     Se retorna (1) si la transicion esta sensibilizada y la operacion fue exitosa.
 * 			   Se retorna (0) si la transicion no esta sensibilizada y la operacion fue exitosa.
 * 			   Se retorna (-EC_datoIncorrecto) si la operacion no fue exitosa.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int RDPG::get_sensitized(size_t p_transicion)
{
	if(p_transicion < transiciones) /* Validacion de direccion correcta en vector. */
	{
		if(RDPGO_DB_MSG) cout << "Info: Transicicion T" << p_transicion << " sensibilizada (" << vEx[p_transicion] << ")."<< endl;
		return vEx[p_transicion];
	}
	else
	{
		cout << "Error: Numero de transicicion incorrecto. La transicicion T" << p_transicion << " no existe en la RDPG: ("<< name <<")." << endl;
		error_code = -EC_datoIncorrecto;
		return -EC_datoIncorrecto;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo indica si la RDPG esta vacia o continene una RDPG cargada.
 *
 * @return     Se retorna TRUE, si el objeto RDPG no contine ninguna RDPG cargada.
 * 			   Se retorna FALSE, si el objeto RDPG contine una RDPG cargada.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
bool RDPG::empty()
{
	if(plazas == 0)
	{
		return true;
	}

	return false;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * METODOS SETTERS
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de realizar el seteo del atributo posVP del objeto RDPG.
 * 			   Se indica el error (-EC_falloPosVP) en el atributo error_code, si no se pudo realizar el seteo.
 *
 * @param[in]  p_posVP  Nuevo valor a aconfigurar sobre el atributo posVP del objeto RDPG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::set_posVP(size_t p_posVP)
{
	if(p_posVP < plazas)
	{
	  posVP = p_posVP;
	}
	else
	{
	  error_code = -EC_falloPosVP;
	  cout << "Error: Fallo el cambio de variable posVP, el valor ingresado supera la cantidad de plazas de la RDPG. \n" << endl;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de realizar el seteo del atributo posVT del objeto RDPG.
 * 			   Se indica el error (-EC_falloPosVT) en el atributo error_code, si no se pudo realizar el seteo.
 *
 * @param[in]  p_posVT  Nuevo valor a aconfigurar sobre el atributo posVT del objeto RDPG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::set_posVT(size_t p_posVT)
{
	if(p_posVT < transiciones)
	{
	  posVT = p_posVT;
	}
	else
	{
	  error_code = -EC_falloPosVT;
	  cout << "Error: Fallo el cambio de variable posVT, el valor ingresado supera la cantidad de transiciones de la RDPG. \n" << endl;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo setea el atributo vdim de un objeto RDPG.
 * 			   Se indica el error (-EC_falloVdim) en el atributo error_code, si no se pudo realizar el seteo.
 *
 * @param[in]  p_vdim  Nuevo valor a configurar en el atributo vdim del objeto RDPG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::set_vdim(size_t p_vdim)
{
	if((p_vdim >= MIN_VDIM) && (p_vdim <= MAX_VDIM))
    {
      vdim = p_vdim;
    }
    else
    {
      error_code = -EC_falloVdim;
      cout << "Error: Fallo el cambio de variable vdim de RDPG: (" << name << "), el valor ingresado supera el maximo o no supera el minimo admitido. \n";
    }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * METODOS DE OBJETOS RDPG
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo realiza la importacion de cada uno de los componentes base de una RDPG a traves del nombre de los archivos enviados por parametro.
 * 			   Se utilizan los metodos import_matrix() e import_vector() para completar la operacion.
 *
 * @param[in]  p_mII   Nombre del archivo asociado al componente mII.
 * @param[in]  p_mIH   Nombre del archivo asociado al componente mIH.
 * @param[in]  p_mIR   Nombre del archivo asociado al componente mIR.
 * @param[in]  p_mIRe  Nombre del archivo asociado al componente mIRe.
 * @param[in]  p_vMI   Nombre del archivo asociado al componente vMI.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::import_RDPG(string p_mII, string p_mIH, string p_mIR, string p_mIRe, string p_vMI)
{
	/* Carga de matrices desde archivos*/
	if(!p_mII.empty())
		import_matrix(ref_mcomp(_mII),p_mII);
	
	if(!p_mIH.empty())
		import_matrix(ref_mcomp(_mIH),p_mIH);
	
	if(!p_mIR.empty())
		import_matrix(ref_mcomp(_mIR),p_mIR);
	
	if(!p_mIRe.empty())
		import_matrix(ref_mcomp(_mIRe),p_mIRe);
	
	if(!p_vMI.empty())
		import_vector(ref_vcomp(_vMI),p_vMI);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de realizar la importacion de cada uno de los elementos de un componente matrix_o de la RDPG desde un archivo asociado.
 *
 * @param      p_mo     Referencia al objeto matrix_o sobre el cual importar los elementos desde el archivo.
 * @param[in]  p_fname  Nombre del archivo asociado desde el que se importan los elementos para el componente matrix_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::import_matrix(matrix_o& p_mo, string p_fname)
{
	ifstream fin; 						/* Objeto para lectura de archivo. */
	string line;						/* Cadena que almacena una linea del archivo. */
	size_t i = 0;

	fin.open(p_fname.c_str());			/* Apertura de archivo. */

	if(!fin.is_open())					/* Si fallo apertura de archivo, se retorna el fallo. */
	{
		cout << "   --> ERROR: No se pudo abrir archivo ( " << p_fname << " ) para su lectura." << endl;
		return;
	}

	getline(fin, line);					/* Captura de primer linea de archivo. */

	while( (!fin.eof()) && (i<plazas))	/* Mientras no se detecte final de archivo.*/
	{
		add_values_mcomp(p_mo, line, i);/* Adicion de valores de linea en fila de matrix_o p_mo. */
		getline(fin, line);				/* Captura de siguiente linea de archivo. */
		i++;
	}

}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de realizar la importacion de cada uno de los elementos de un componente vecotr_o de la RDPG desde un archivo asociado.
 *
 * @param      p_vo     Referencia al objeto vector_o sobre el cual importar los elementos desde el archivo.
 * @param[in]  p_fname  Nombre del archivo asociado desde el que se importan los elementos para el componente vector_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::import_vector(vector_o& p_vo, string p_fname)
{
	ifstream fin; 						/* Objeto para lectura de archivo. */
	string line;						/* Cadena que almacena una linea del archivo. */

	fin.open(p_fname.c_str());			/* Apertura de archivo. */

	if(!fin.is_open())					/* Si fallo apertura de archivo, se retorna el fallo. */
	{
		cout << "   --> ERROR: No se pudo abrir archivo ( " << p_fname << " ) para su lectura." << endl;
		return;
	}

	getline(fin, line);					/* Captura de primer linea de archivo. */

	while( !fin.eof())					/* Mientras no se detecte final de archivo.*/
	{
		add_values_vcomp(p_vo, line);	/* Adicion de valores de primer linea en vector_o p_vo. */
		getline(fin, line);				/* Captura de siguiente linea de archivo. */
	}

}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de asignar los valores de una fila del componente matrix_o enviado por parametro.
 *
 * @param      p_mo    Referencia al objeto matrix_o sobre el cual asignar los valores.
 * @param[in]  p_line  Cadena de caracter con cada uno de los elementos a asignar.
 * @param[in]  p_fila  Numero de fila asociada a componente matrix_o sobre la que se asignan los elementos.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::add_values_mcomp(matrix_o& p_mo, string p_line, size_t p_fila)
{
	istringstream dline(p_line);		/* Objeto para tratar string como un stream de entrada. Se inicia con dline (data line).*/
	int element;						/* Entero para almacenar elementos de matriz.*/
	size_t j=0;

	while(dline >> element)
	{
		p_mo[p_fila][j] = element;		/* Asignacion de elemento de string en matrix_o. */
		j++;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de asignar los valores de un componente vector_o enviado por parametro.
 *
 * @param      p_vo    Referencia al objeto vector_o sobre el que se desea realizar la asignacion.
 * @param[in]  p_line  Cadena de caracter con cada uno de los elementos a asignar.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::add_values_vcomp(vector_o& p_vo, string p_line)
{
	istringstream dline(p_line);		/* Objeto para tratar string como un stream de entrada. Se inicia con dline (data line).*/
	int element;						/* Entero para almacenar elementos de vector.*/
	size_t i=0;

	while(dline >> element)
	{
		p_vo[i] = element;				/* Asignacion de elemento de string en vector_o. */
		i++;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de actualizar un valor del componente vector_o vG del objeto RDPG.
 *
 * @param[in]  p_pos    Posicion del elemento sobre el componente vG que se desea actualizar.
 * @param[in]  p_valor  Valor que se desea actualizar sobre la posicion del vector vG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::add_value_vG(size_t p_pos, int p_valor)
{
	if(p_pos < transiciones)
	{
		if((p_valor==1) || (p_valor==0))
		{
			vG[p_pos] = p_valor;
			update_vEx();
		}
		else
		{
			error_code = -EC_datoIncorrecto;
			cout << "Error: No se pudo actualizar el vector vG. Dato incorrecto." << endl;		
		}			
	}
	else
	{
		error_code = -EC_posicionIncorrecta;
		cout << "Error: No se pudo actualizar el vector vG. Posicion incorrecta." << endl;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo actualiza los componentes vMA, vE, vW, vQ, vB, vL, vA, vEx y vHD de una RDPG luego de que se asignan los componentes mII, mIR,
 * mIH y vMI.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::update_work_components()
{
  update_vE();
  update_vW();
  update_vQ();
  update_vB();
  update_vL();
  update_vA();
  update_vEx();
  update_vHD();
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Este metodo se encarga de actualizar el vector vE, una vez que se completo la carga del vector vMI. La funcion dispara todas las transiciones 
 * en modo implicito para determinar que transiciones estan sensibilizadas y cuales no lo estan, de esta manera se obtiene el vector de transiciones 
 * sensibilizadas.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::update_vE()
{
  int i;
  
  if( !vE.empty())
  {
  	  for (i = 0; i < transiciones; i++)
  	  {
  	      if(shoot(i,SHOT_MODE_I) == SHOT_OK){
  	        vE[i] = 1;
  	      }
  
  	      else{
  	        vE[i] = 0;
  	      }
  	  }
  	  if(RDPGO_DB_MSG) cout << " Actualizacion de componente vE con exito!!!\n";
   }
   else
   {
   		cout << " Error: No es posible actualizar el vector vE!\n";
   }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Este metodo se encarga de actualizar el vector vQ, cuyos elementos qi son qi -> 0 si M(pi) != 0 o qi -> 1 en el resto de los casos estos 
 * valores se toman a partir del vector de marcado actual vMA.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::update_vQ()
{
  int i;

  if((!vQ.empty()) && (!vMA.empty())) /* Si existen vectores afectados. Actualizo valores de vQ. */
  {
      for (i = 0; i < vMA.size(); i++)
      {
          if(vMA[i] != 0)
          {
            vQ[i] = 0;
          }

          else
          {
            vQ[i] = 1;
          }
      }
  }
  else
  {
    cout << "Error: No es posible actualizar componente vQ! \n";
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Este metodo se encarga de actualizar el vector vW, cuyos elementos wi son wi -> 1 si M(pi) != 0 o wi-> 0 en el resto de los casos. Los
 * valores se toman a partir del vector de marcado actual vMA.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::update_vW()
{
  int i;

  if((!vW.empty()) && (!vMA.empty())) /* Si existen componentes afectados. Actualizo valores de vW. */
  {
      for (i = 0; i < vMA.size(); i++)
      {
          if(vMA[i] != 0){
            vW[i] = 1;
          }

          else
          {
            vW[i] = 0;
          }
      }
  }
  else
  {
    cout << "Error: No es posible actualizar componente vW!!!\n";
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Este metodo se encarga de actualizar el vector vB, una vez que se completo la carga de vMI, mH y vQ.
 * El vector vB que carga la funcion, es un vector de valores binarios de dimension mx1 (siendo m la cantidad de transiciones existentes en la RDP) que
 * indica con un cero cuales transiciones estan inhibidas por el arco inhibidor y con un uno las que no.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::update_vB()
{
   int i, j;

   if((!vB.empty()) && (!vQ.empty()) && (!mIH.empty())) /* Si existen componentes afectados. Actualizo valores vB. */
   {
      /* Inicializacion de valores en sensibilizados (uno binario) sobre vector B*/
      for(i=0; i < transiciones; i++)
        if(vB[i]!=1)
        {
          vB[i]=1;
        }

      /* Calculo de valores inhibidos (cero binario) sobre vector B*/
      for(i=0; i< transiciones; i++) 	/* lectura de transiciones de RDPG */
        for(j=0; j< plazas; j++) 		/* lectura de plazas de RDPG */
            if(mIH[j][i] == 1)
            {
               vB[i] = mIH[j][i] & vQ[j];
               if(vB[i] == 0)			/* Si se detecto inhibicion, salir! */
               	break;
            }

      if(RDPGO_DB_MSG) cout << "Info: Actualizacion de vector B con exito!!!\n";
  }
  else
  {
    cout << "Error: No es posible actualizar componente vB!!!\n";
  }

}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Este metodo se encarga de actualizar el vector vL de una RDPG_o, una vez que se completo la carga de vMI, mR y vW. El vector
 * vL que carga la funcion, es un vector de valores binarios de dimension mx1 (siendo m la cantidad de transiciones existentes en la
 * RDP) que indica con un cero cuales transiciones estan inhibidas por el arco lector y un uno las que no.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::update_vL()
{
    int i, j;

    if((!vL.empty()) && (!vW.empty()) && (!mIR.empty())) /* Si existen componentes afectados. Actualizo valores vL. */
    {
      /* Inicializacion de valores en sensibilizados (uno binario) sobre vector L*/
      for(i=0; i < transiciones; i++)
        if(vL[i]!=1)
        {
          vL[i]=1;
        }

      /* Calculo de valores inhibidos (cero binario) sobre vector L*/
      for(i=0; i< transiciones; i++) 	/* lectura de transiciones de RDPG */
        for(j=0; j< plazas; j++) 		/* lectura de plazas de RDPG */
            if(mIR[j][i] == 1)
            {
              vL[i] = mIR[j][i] & vW[j];
              if(vL[i] == 0)			/* Si se detecto inhibicion, salir! */
               	break;
            }

      if(RDPGO_DB_MSG) cout << "Info: Actualizacion de vector L con exito!!!\n";
    }
    else
	{
		cout << "Error: No es posible actualizar componente vL!!!\n";
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Este metodo se encarga de actualizar el vector vG de una RDPG_o enviada por parametro. Se inicializan todos los elementos del vector a uno.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::update_vG()
{
    int i, j;

    if(!vG.empty()) /* Si existen componentes afectados. Actualizo valores vG. */
    {
      /* Inicializacion de valores en sensibilizados (uno binario) sobre vector G. */
      cpy_vector(vG,vector_o (vG.size(), 1));
      
      if(RDPGO_DB_MSG) cout << "Info: Actualizacion de vector G exitosamente!!!\n";
    }
    else
    {
      cout << "Error: No es posible actualizar componente vector G!\n";
    }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Este metodo se encarga de actualizar el vector vA de transiciones reset, de acuerdo con la teoria de las RDPG. Donde sus valores son todos
 *             uno salvo para las transiciones con arco reset entrante, cuyo valor es el numero indicado por la plaza que se conecta con la transicion (vMA[plaza]).
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::update_vA()
{
    int i, j;

    if((!vA.empty()) && (!mIRe.empty()) && (!vMA.empty())) /* Si existen componentes afectados. Actualizo valores vA. */
    {
      vector_o v_marca(vA.size());                	/* Creo vector auxiliar para funcion de marca. Se inicializa a cero por defecto.*/

      /* Inicializacion de valores en sensibilizados (uno binario) sobre vector A. */
      cpy_vector(vA, vector_o(vA.size(),1));

      /* Calculo de plazas con arco reset saliente.*/
      for(i=0; i< transiciones; i++)  				/* lectura de transiciones de RDPG */
      {
        for(j=0; j<plazas; j++)   					/* lectura de plazas de RDPG */
        {
            if(mIRe[j][i] == 1)  					/* Si existe plaza con arco reset saliente.*/
            {
              v_marca[i] = vMA[j]; 					/* Guardo valor en vector marca. */
            }
        }
      }

      /* Calculo de vector A. */
      for(i=0; i< transiciones; i++)  /* lectura de transiciones de RDPG */
      {
        if(v_marca[i] > 1)  /* Elemento de vector A es componente v_marca[i] si no es cero y mayor que 1. */
          vA[i]=v_marca[i];
        
        /* Elemento de vector A es uno si v_marca[i] es cero. Esto se realiza en inicializacion de vA a uno. */  
      }

      if(RDPGO_DB_MSG) cout << "Info: Actualizacion de vector A exitosamente!!!\n";
    }
    else
    {
      cout << "Error: No es posible actualizar componente vector A!!!\n";
    }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Este metodo se encarga de actualizar el vector vEx de una RDPG_o, una vez que se completo la carga de los vectores vE, vB y vL.
 * El vector vEx que carga la funcion, es un vector de valores binarios de dimension mx1 (siendo m la cantidad de transiciones existentes en la RDPG)
 * que indica con un cero cuales transiciones estan inhibidas por la conjuncion de todos arcos de la RDPG y con un uno las que no.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::update_vEx()
{
    int i, j;

    if((!vEx.empty()) && (!vE.empty()) && (!vB.empty()) && (!vL.empty()) && (!vG.empty())) /* Si existen componentes afectados. Actualizo valores de vEx. */
    {
      /* Calculo de la conjuncion logica de todos los vectores para generar el vector Ex*/
      for(i=0; i < transiciones; i++)
      {
          vEx[i] = vE[i] & vB[i] & vL[i] & vG[i];
      }

      //if(RDPGO_DB_MSG) cout << "Info: Actualizacion de vector Ex con exito!!!\n";
    }
    else
    {
    	cout << "Error: No es posible actualizar componente vector Ex!!!\n";
    }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de actualizar el vector_o vHD, que es el vector de los hilos a despertar.
 * 			   El mismo se forma a partir de la conjuncion entre el vector de transiciones sensibilizadas extendido (vEx) con el vector de hilos en colas de
 * 			   las variables de condicion de la RDPG (es una variable de condicion por transicion de la RDPG).
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::update_vHD()
{
	for(size_t i=0; i < transiciones; i++)
	{
		vHD[i]= (int)(vEx[i] * vHQCV[i]);
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Este metodo se encarga de realizar el disparo de una transicion (p_idT) sobre una RDPG enviada por parametro. La funcion analiza si el
 * numero de la transicion enviada por parametro p_idT existe en la RDPG, en caso se existir se valida si la transicion esta sensibilizada y en caso
 * de estar sensibilizada se dispara la transicion indicada. Para cualquier otro caso se detalla el inconveniente de la falla del disparo de la transicion.
 * 
 * @param[in]  p_idT      Entero que reresenta el numero de transicion a disparar sobre la RDPG enviada por parametro.
 * @param[in]  p_mode   Indica el modo de disparo de acuerdo a la enum SHOT_MODE.
 *                    - SHOT_MODE_I: modo implicito, no actualiza el vector de marcado actual vMA.
 *                    - SHOT_MODE_E: modo explicito, actualiza el vector de marcado actual vMA.
 *
 * @return El retorno es:
 *      	- SHOT_FAILED: si no se puede realizar disparo sobre la RDPG_o enviada por parametro (la transicion enviada no esta sensibilizada para disparar en la RDPG).
 *      	- SHOT_OK: si puedo realizar el disparo de la transicion enviada sobre RDPG (la transicion enviada estaba sensibilizada para ese disparo en la RDPG).
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int RDPG::shoot_rdpg(int p_idT, SHOT_MODE p_mode)
{  
    if((p_idT > -1) && (p_idT < transiciones)) 	/* Existe transicion indicada ? Disparar p_idT */
    {
      if(vEx[p_idT] == ST_SENSITIZED) 			/* Transicion sensibilizada ? */
      {
        int j, auxMN;

        if(mD[p_idT][p_idT]==1)   				/* Verificacion de disparo valido. */
        {
          for (j = 0; j < plazas; j++)			/* Actualizacion de marcas de acuerdo a disparo de transicion p_idT. */
          {
            if(p_mode == SHOT_MODE_E) // modo de disparo explicito: se actualiza el nuevo estado en el vector MN, si el disparo es exitoso se actualiza MA posteriormente.
            {
                //vMN[j] = vMA[j] + mII[j][i]*mD[p_idT][i]*vEx[i];
                vMN[j] = vMA[j] + ( (mII[j][p_idT]) * ( ((mD[p_idT][p_idT]) & (vEx[p_idT])) * vA[p_idT]) );
                if(vMN[j]< 0)
                {
                  if(RDPGO_DB_MSG) cout << "Error: Fallo disparo de transicion T" << p_idT << "!!!\n";
                  shot_result = (int)SHOT_FAILED;
                  vUDT[p_idT] = (int)SHOT_FAILED;
                  return SHOT_FAILED;
                }
            }
            else // modo de disparo implicito: no interesa almacenar el estado de MN pero si saberlo.
            {
                // Se almacena de manera auxiliar los valores del nuevo estado de la transicion, para verificar si esta sensibilizada
                //auxMN = vMA[j] + mII[j][i]*mD[p_idT][i]*vEx[i];
                auxMN = vMA[j] + ( (mII[j][p_idT]) * ( ((mD[p_idT][p_idT]) & (vEx[p_idT])) * vA[p_idT]) );
                if(auxMN < 0)
                  return SHOT_FAILED;
            }
          }
        }

        /* Actualizacion de marcado actual y vectores*/
        if(p_mode == SHOT_MODE_E)
        {
          cpy_vector(vMA, vMN);

          /* Se actualiza el resto de vectores de RDPG si el disparo es explicito. */
          update_work_components();

          if(RDPGO_DB_MSG) cout << "Info: Se disparo exitosamente la transicion T" << p_idT << "!!!\n";
          shot_result = (int)SHOT_OK;
          vUDT[p_idT] = (int)SHOT_OK;
        }

        return SHOT_OK;
      }
      else
      { 
        /* Se limpia vMN a cero ... */
        cpy_vector(vMN, vector_o(vMN.size()));
        if(RDPGO_DB_MSG) cout << "Error: Transicion T" << p_idT << " no sensibilizada!!!\n";
        shot_result = (int)SHOT_FAILED;
        vUDT[p_idT] = (int)SHOT_FAILED;
        //SHOT_OK;
        return SHOT_FAILED; 
      }   
    }
    else
    {
      error_code = -EC_transicionInexistente;
      if(RDPGO_DB_MSG) cout << "Error: El disparo no fue exitoso. Se indico una transicion que no existe en la RDPG: (" << name << "!!!\n";
      shot_result = (int)SHOT_FAILED;
      vUDT[p_idT] = (int)SHOT_FAILED;
      return SHOT_FAILED;
    }

  /* Si no existe RDPG afectada al disparo de transicion indicada se retorna fallo de disparo. */
  return SHOT_FAILED;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Este metodo realiza el disparo sobre la RDP de acuerdo a un vector de disparo indicado por parametro. Se verifica la sensibilidad de la
 * RDP para el disparo de la transicion y se actualiza o no el marcado actual de acuerdo a si el disparo es exitoso o no.
 * Se permite realizar el disparo de una transicion en dos modos, implicita o explicitamente indicado por parametro.
 * 
 * @param[in]   id_d    Identificador de la transicion a disparar sobre RDP.
 * @param[in]   p_mode  Modo con el que se desea realizar el disparo sobre la RDPG_o. Los modos pueden ser:
		        - SHOT_MODE_I: modo implicito, no actualiza el vector de marcado actual MA. Permite visualizar el estado de cualquiera de los disparos
		        sin actualizar los vectores de marcado.
		        - SHOT_MODE_E: modo explicito, actualiza el vector de marcado actual MA ante cualquier disparo a realizar.
 *
 * @return     El retorno es:
 *        		- SHOT_FAILED: si no se puede realizar disparo sobre RdP (transicion no sensibilizada en la RDPG_o).
 *        		- SHOT_OK: si puedo realizar disparo sobre RDP (transicion sensibilizada en la RDPG_o).
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int RDPG::shoot(int id_d, SHOT_MODE p_mode)
{
  /* Disparar */
  int j, auxMN;

    if( (id_d < transiciones) && (id_d >= 0) ) /* Si id_d es valido para RDP. */
    {
	    /* Se valida la transicion a disparar en transiciones de RDP. */
	    if(mD[id_d][id_d]==1)
	    {
	      /* Se actualizan todas las plazas afectados por el disparo de transicion, todo se registra en vector vMN de la RDP.*/
	      for (j = 0; j < plazas; j++)
	      {
	        if(p_mode == SHOT_MODE_E)
	        {
	            vMN[j] = vMA[j] + mII[j][id_d]*mD[id_d][id_d];
	            if(vMN[j]< 0)
	              return SHOT_FAILED;
	        }
	        else /* modo de disparo implicito no intera almacenar el estado de MN ni MA pero si saberlo */
	        {
	            /* Se almacena de manera auxiliar los valores del nuevo estado de la transicion, para verificar si esta sensibilizada */
	            auxMN = vMA[j] + mII[j][id_d]*mD[id_d][id_d];
	            if(auxMN < 0)
	              return SHOT_FAILED;
	        }
	      }
	    }
	  
	    /* Si el disparo es en modo explicito se actualiza marcado actual. */
	    if(p_mode == SHOT_MODE_E)
	    {
	      for (j = 0; j < plazas; j++){
	        vMA[j] = vMN[j];
	      }
	    }
      	
      	return SHOT_OK;
    }
  
  /* Si id_d es un numero de transicion no valido se retorna sin ninguna modificacion.*/
  /* Si no existen objetos RDPG afectado a un disparo de red no se realiza ninguna operacion. */
  return SHOT_FAILED;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de imprimir informacion asociada al objeto RDPG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::print_RDPGinfo() const
{
	print_line('-');
	cout << " Informacion de RDPG: " << name << endl;
	print_line('-');
	cout << " Nombre de RDPG: " << get_name() << endl;
	cout << " Identificador de red: " << get_objID() << endl;
	cout << " Numeros de plazas: " << get_plazas() << endl;
	cout << " Numero de transiciones: " << get_transiciones() << endl;
	cout << " Posicion de plaza inicial para visualizacion: " << get_posVP() << endl;
	cout << " Posicion de transicion inicial para visualizacion: " << get_posVT() << endl;
	cout << " Dimension de visualizacion de componentes: " << get_vdim() << endl;
	cout << " Memoria reservada por RDPG: " << get_RDPGSize() << " bytes." << endl;
	//cout << " Ultimo error de codigo: " << get_errorCode() << endl;
	print_line('-');
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * METODOS DE COMPOENTES matrix_o DE RDPG
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo analiza los componentes matrix_o y retorna la referencia de uno de ellos si coincide con el id enviado por parametro.
 *
 * @param[in]  p_mcomp  Identificador del componente matrix_o que se desea retornar.
 *
 * @return     Se retorna la referencia del objeto matrix_o asociada al identificador enviado por parametro.
 * 			   En caso de error se indica en el campo error_code con el error -EC_referenciaNula.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
matrix_o& RDPG::ref_mcomp(ID_MCOMPONENT p_mcomp)
{
	error_code = EC_NULL;
	if((p_mcomp>=ID_MC_INIT) && (p_mcomp < ID_MC_END))
	{
		if(p_mcomp == _mII)
			return mII;
		else if(p_mcomp == _mIH)
			return mIH;
		else if(p_mcomp == _mIR)
			return mIR;
		else if(p_mcomp == _mIRe)
			return mIRe;
		else if(p_mcomp == _mD)
			return mD;
	}
	
	error_code = -EC_referenciaNula;
	if(RDPGO_DB_MSG) cout<< "Error: En la RDPG: (" << name << ") no existe la referencia de matrix_o indicada como parametro." << endl;
	//return (matrix_o&)0;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encargar de cargar un componente matrix_o con la matriz identidad.
 *
 * @param[in]  p_mcomp  Identificador del objeto matrix_o de la RDPG sobre el que se desea cargar la matriz identidad.
 * 			 			Si no de encuentra el componente de la RDPG se avisa en el atributo error_code con -EC_componeteNoValido.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::identity_matrix(ID_MCOMPONENT p_mcomp)
{
	size_t i, j;
	
	if((p_mcomp>=ID_MC_INIT) && (p_mcomp < ID_MC_END))
	{	
		matrix_o& comp = ref_mcomp(p_mcomp);

		reload_matrix(p_mcomp, 0);

		for(i = 0; i<comp.size(); i++) {
			for(j = 0; j<comp[0].size(); j++)
				if(i==j){
					comp[i][j] = 1;
					break;
				}
		}
		/*else
		{
			cout << "Fallo la operacion identity_matrix para RDPG: ("<< name << ")." << endl;
		}*/
	}
	else
	{
		error_code = -EC_componeteNoValido;
		cout << "Fallo la operacion identity_matrix para RDPG: ("<< name << "). No existe id: " << p_mcomp << " enviado por parametro." <<endl;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de limpiar un componente matrix_o de la RDPG.
 *
 * @param[in]  p_mcomp  Identificador del componente matrix_o de la RDPG a limpiar.
 * 			 			Si no de encuentra el componente de la RDPG se avisa en el atributo error_code con -EC_componeteNoValido.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::clean_matrix(ID_MCOMPONENT p_mcomp)
{
	size_t i, j;

	if((p_mcomp>=ID_MC_INIT) && (p_mcomp < ID_MC_END))
	{
		matrix_o& comp = ref_mcomp(p_mcomp);
	
		for(i = 0; i<comp.size(); i++) {
			for(j = 0; j<comp[0].size(); j++)
				comp[i][j] = 0;
		}
	}
	else
	{
		error_code = -EC_componeteNoValido;
		cout << "Fallo la operacion clean_matrix para RDPG: ("<< name << "). No existe id: " << p_mcomp << " enviado por parametro." <<endl;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encargar de recargar un componente matrix_o de la RDPG con un valor literal enviado por parametro.
 *
 * @param[in]  p_mcomp  Identificador del componente matrix_o de la RDPG que se desea recargar.
 * @param[in]  p_valor  Valor literal que se desea cargar en cada uno de los elementos del componente matrix_o.
 * 			 			Si no de encuentra el componente de la RDPG se avisa en el atributo error_code con -EC_componeteNoValido.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::reload_matrix(ID_MCOMPONENT p_mcomp, int p_valor)
{
	size_t i, j;

	if((p_mcomp>=ID_MC_INIT) && (p_mcomp < ID_MC_END))
	{
		matrix_o& comp = ref_mcomp(p_mcomp);

		for(i = 0; i<comp.size(); i++) {
			for(j = 0; j<comp[0].size(); j++)
					comp[i][j] = p_valor;
		}
	}
	else
	{
		error_code = -EC_componeteNoValido;
		cout << "Fallo la operacion clean_matrix para RDPG: ("<< name << "). No existe id: " << p_mcomp << " enviado por parametro." <<endl;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de imprimir cualquier componete matrix_o de la RDPG.
 *
 * @param[in]  p_mcomp  Identificador del componente matrix_o de la RDPG a imprimir.
 * 			 			Si no de encuentra el componente de la RDPG se avisa en el atributo error_code con -EC_componeteNoValido.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::print_mcomp(ID_MCOMPONENT p_mcomp)
{
	size_t i, j, vp, vt;

	print_headerT();

	if((p_mcomp>=ID_MC_INIT) && (p_mcomp < ID_MC_END) && (p_mcomp != _mD))
	{
		matrix_o& comp = ref_mcomp(p_mcomp);

		for(i = posVP, vp=0; (i<comp.size())&&(vp<vdim); i++, vp++) {
			if(i < 10)
				cout << " P"<< i << right << setw(3) << "|";
			else if(i < 100)
				cout << " P"<< i << right << setw(2) << "|";
			else if( i < MAX_PLACES)
				cout << " P"<< i << right << setw(1) << "|";
			for(j = posVT, vt=0; (j<comp[0].size())&&(vt<vdim); j++, vt++){
				if(j < 10)
					cout << right << setw( 3 ) << comp[i][j];
				else if(j < 100)
					cout << right << setw( 4 ) << comp[i][j];
				else if( j < MAX_TRANSITIONS)
					cout << right << setw( 5 ) << comp[i][j];
			}
			cout << endl;
		}
	}
	else if (p_mcomp == _mD)
	{
		matrix_o& comp = ref_mcomp(p_mcomp);

		for(i = posVP, vp=0; (i<comp.size())&&(vp<vdim); i++, vp++) {
	 		if(i < 10)
				cout << " T"<< i << right << setw(3) << "|";
			else if(i < 100)
				cout << " T"<< i << right << setw(2) << "|";
			else if( i < MAX_PLACES)
				cout << " T"<< i << right << setw(1) << "|";
			for(j = posVT, vt=0; (j<comp[0].size())&&(vt<vdim); j++, vt++){
				if(j < 10)
					cout << right << setw( 3 ) << comp[i][j];
				else if(j < 100)
					cout << right << setw( 4 ) << comp[i][j];
				else if( j < MAX_TRANSITIONS)
					cout << right << setw( 5 ) << comp[i][j];
			}
			cout << endl;
		}
	}
	else
	{
		error_code = -EC_componeteNoValido;
		cout << "Fallo la operacion print_mcomp para RDPG: ("<< name << "). No existe id: " << p_mcomp << " enviado por parametro." <<endl;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de imprimir la cabecera de las transiciones de acuerdo con las configuraciones de los atributos vdim y posVT.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::print_headerT()
{
	size_t vt; 				/* Numero de transiciones visualizadas.*/
	if(((posVT+vdim)>=100) && (vdim>14) && (posVT<(MAX_TRANSITIONS-14)))
		print_line('-', 165);
	else if( ((posVT+vdim)<100) && (vdim>18) )
		print_line('-', 165);
	else
		print_line('-');
 	
 	cout << "     |  ";

 	for(size_t i=posVT, vt=0; (i < transiciones)&&(vt < vdim); i++, vt++)
 	{
 		if(i < 10)
 			cout << "T" << i << right << setw( 2 );
 		else if(i < 100)
 			cout << "T" << i << right << setw( 2 );
 		else if(i < MAX_TRANSITIONS)
 			cout << "T" << i << right << setw( 2 );
 	}
 	cout << endl;
 	
 	if(((posVT+vdim)>=100) && (vdim>14) && (posVT<(MAX_TRANSITIONS-14)))
		print_line('-', 165);
	else if( ((posVT+vdim)<100) && (vdim>18) )
		print_line('-', 165);
	else
		print_line('-');
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de imprimir la cabecera de las plazas de acuerdo con las configuraciones de los atributos vdim y posVP.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::print_headerP()
{
	size_t vp; 				/* Numero de plazas visualizadas.*/
	if(((posVP+vdim)>=100) && (vdim>14) && (posVP<(MAX_PLACES-14)))
		print_line('-', 165);
	else if( ((posVP+vdim)<100) && (vdim>18) )
		print_line('-', 165);
	else
		print_line('-');

 	cout << "     |  ";

 	for(size_t i=posVP, vp=0; (i < plazas)&&(vp < vdim); i++, vp++)
 	{
 		if(i < 10)
 			cout << "P" << i << right << setw( 2 );
 		else if(i < 100)
 			cout << "P" << i << right << setw( 2 );
 		else if(i < MAX_PLACES)
 			cout << "P" << i << right << setw( 2 );
 	}
 	cout << endl;

 	if(((posVP+vdim)>=100) && (vdim>14) && (posVP<(MAX_PLACES-14)))
		print_line('-', 165);
	else if( ((posVP+vdim)<100) && (vdim>18) )
		print_line('-', 165);
	else
		print_line('-');
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de realizar una reasignacion de tamaño en los componentes matrix_o del objeto RDPG de acuerdo al numero 
 * 			   de plazas y tranciones.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::resize_allmatrixs()
{
	resize_matrix(_mII, plazas, transiciones);
	resize_matrix(_mIH, plazas, transiciones);
	resize_matrix(_mIR, plazas, transiciones);
	resize_matrix(_mIRe, plazas, transiciones);
	resize_matrix(_mD, transiciones, transiciones);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de reasignar el tamaño de memoria a un componente matrix_o de la RDPG.
 *
 * @param[in]  p_mcomp  Identificador del componente matrix_o de la RDPG a realizar la reasignacion.
 * @param[in]  p_row    Numero de filas a reasignar en el componente matrix_o.
 * @param[in]  p_col    Numero de columnas a reasignar en el componente matrix_o-
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::resize_matrix(ID_MCOMPONENT p_mcomp, size_t p_row, size_t p_col)
{
	size_t i;
	matrix_o& comp = ref_mcomp(p_mcomp);

	if(error_code == -EC_referenciaNula)
	{
		cout << "ERROR: No se pudo realizar la operacion de reasignacion sobre el componente indicado de la RDPG." << endl;
		return;
	}

	comp.resize(p_row);				/* Redimension de filas de matriz. */

	for(i=0; i< comp.size(); i++)
	{
		comp[i].resize(p_col);		/* Redimension de columnas de matriz. */
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * METODOS DE COMPOENTES vector_o DE RDPG
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo analiza los componentes vector_o y retorna la referencia de uno de ellos si coincide con el id enviado por parametro.
 *
 * @param[in]  p_vcomp  Identificador de componente vector_o de la RDPG.
 *
 * @return     Se retorna la referencia de un objeto vector_o, si la operacion tiene exito.
 * 			   Si la operacion no tiene exito se acisa por el atributo error_code con el valor -EC_referenciaNula.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
vector_o& RDPG::ref_vcomp(ID_VCOMPONENT p_vcomp)
{
	error_code = EC_NULL;
	if((p_vcomp>=ID_VC_INIT) && (p_vcomp < ID_VC_END))
	{
		if(p_vcomp == _vMI)
			return vMI;
		else if(p_vcomp == _vMA)
			return vMA;
		else if(p_vcomp == _vMN)
			return vMN;
		else if(p_vcomp == _vE)
			return vE;
		else if(p_vcomp == _vQ)
			return vQ;
		else if(p_vcomp == _vW)
			return vW;
		else if(p_vcomp == _vB)
			return vB;
		else if(p_vcomp == _vL)
			return vL;
		else if(p_vcomp == _vG)
			return vG;
		else if(p_vcomp == _vA)
			return vA;
		else if(p_vcomp == _vUDT)
			return vUDT;
		else if(p_vcomp == _vEx)
			return vEx;
		else if(p_vcomp == _vHQCV)
			return vHQCV;
		else if(p_vcomp == _vHD)
			return vHD;
	}
	
	error_code = -EC_referenciaNula;
	cout<< "Error: En la RDPG: (" << name << ") no existe la referencia de vector_o indicado como parametro." << endl;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta metodo se encarga de limpiar un componente vector_o de la RDPG.
 *
 * @param[in]  p_vcomp  Identificador del componente vector_o de la RDPG a limpiar.
 * 			 			Si no de encuentra el componente de la RDPG se avisa en el atributo error_code con -EC_componeteNoValido.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::clean_vector(ID_VCOMPONENT p_vcomp)
{
	size_t i;

	if((p_vcomp>=ID_VC_INIT) && (p_vcomp < ID_VC_END))
	{
		vector_o& comp = ref_vcomp(p_vcomp);
	
		for(i = 0; i<comp.size(); i++) {
				comp[i] = 0;
		}
	}
	else
	{
		error_code = -EC_componeteNoValido;
		cout << "Fallo la operacion clean_vector para RDPG: ("<< name << "). No existe id: " << p_vcomp << " enviado por parametro." <<endl;
	}	
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo actualiza todos los elementos de un componente vector_o de una RDPG con el valor literal enviado por parametro.
 *
 * @param[in]  p_vcomp  Identificador del componente vector_o de la RDPG que se desea actualizar.
 * @param[in]  p_valor  Valor literal a asignar en cada uno de los elementos del componente vector_o de la RDPG.
 * 			 			Si no de encuentra el componente de la RDPG se avisa en el atributo error_code con -EC_componeteNoValido.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::reload_vector(ID_VCOMPONENT p_vcomp, int p_valor)
{
	size_t i;

	if((p_vcomp>=ID_VC_INIT) && (p_vcomp < ID_VC_END))
	{
		vector_o& comp = ref_vcomp(p_vcomp);

		for(i = 0; i<comp.size(); i++) {
					comp[i] = p_valor;
		}
	}
	else
	{
		error_code = -EC_componeteNoValido;
		cout << "Fallo la operacion reload_vector para RDPG: ("<< name << "). No existe id: " << p_vcomp << " enviado por parametro." <<endl;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo imprime los elementos de un componente vector_o de acuerdo a las configuraciones de los atributos vdim, posVP y posVT.
 *
 * @param[in]  p_vcomp  Identificador del componente vector_o de la RDPG que se desea imprimir.
 * 			 			Si no de encuentra el componente de la RDPG se avisa en el atributo error_code con -EC_componeteNoValido.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::print_vcomp(ID_VCOMPONENT p_vcomp)
{
	size_t i, v;		/* v: elementos visualizados. */

	if((p_vcomp>=_vMI) && (p_vcomp <= _vW) && (p_vcomp != _vE))
	{
		print_headerP();
		vector_o& comp = ref_vcomp(p_vcomp);

		cout << " "<< right << setw(5) << "|";

		for(i = posVP, v=0; (i<comp.size())&&(v<vdim); i++, v++) {
			if(i < 10)
				cout << right << setw( 3 ) << comp[i];
			else if(i < 100)
				cout << right << setw( 4 ) << comp[i];
			else if( i < MAX_TRANSITIONS)
				cout << right << setw( 5 ) << comp[i];
		}
		cout << endl;
	}
	else if ( (p_vcomp == _vE) || ((p_vcomp>_vW) && (p_vcomp < ID_VC_END)) )
	{
		print_headerT();
		vector_o& comp = ref_vcomp(p_vcomp);

		cout << " "<< right << setw(5) << "|";

		for(i = posVT, v=0; (i<comp.size())&&(v<vdim); i++, v++) {
			if(i < 10)
				cout << right << setw( 3 ) << comp[i];
			else if(i < 100)
				cout << right << setw( 4 ) << comp[i];
			else if( i < MAX_TRANSITIONS)
				cout << right << setw( 5 ) << comp[i];
		}
		cout << endl;
	}
	else
	{
		error_code = -EC_componeteNoValido;
		cout << "Fallo la operacion print_vcomps para RDPG: ("<< name << "). No existe id: " << p_vcomp << " enviado por parametro." <<endl;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo se encarga de realizar una reasignacion de tamaño en los componentes vector_o del objeto RDPG de acuerdo al numero 
 * 			   de plazas y tranciones.  
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::resize_allvectors()
{
	vMI.resize(plazas);
	vMA.resize(plazas);
	vMN.resize(plazas);
	vE.resize(transiciones);
  	vQ.resize(plazas);
  	vW.resize(plazas); 
  	vL.resize(transiciones,1);
  	vB.resize(transiciones,1);
  	vG.resize(transiciones,1); 
  	vA.resize(transiciones); 
  	vUDT.resize(transiciones); 
  	vEx.resize(transiciones);
  	vHQCV.resize(transiciones);
  	vHD.resize(transiciones);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Este metodo copia todos los elementos de un vector origen hacia un vector destino.
 *
 * @param      v_dst  Referencia del objeto vector_o destino, sobre el cual se actualizan sus elementos.
 * @param[in]  v_src  Referencia del objeto vector_o origen, sobre el cual se copian sus elementos.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void RDPG::cpy_vector(vector_o& v_dst, const vector_o& v_src)
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