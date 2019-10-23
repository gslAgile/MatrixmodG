#include <iostream>
#include <fstream>

using namespace std;

void gen_matrix_identity(ofstream& p_file, size_t const p_filas, size_t const p_col);
void gen_vector(ofstream& p_file, size_t const p_filas);



int main()
{
	ofstream fout("matrix1000.txt"); 		/* Archivo de escritura. */
	ofstream foutv("vector1000.txt");

	size_t filas = 1000;
	size_t columnas = 1000;

	gen_matrix_identity(fout, filas, columnas);

	gen_vector(foutv, filas);

	return 0;
}


void gen_matrix_identity(ofstream& p_file, size_t const p_rows, size_t const p_cols)
{
	size_t i, j;

	for(i = 0; i < p_rows; i++)
	{
		for(j = 0; j < p_cols-1; j++)
		{
			if(i == j)
				p_file << 1 << " ";
			else
				p_file << 0 << " ";
		}

		if(i == j)
				p_file << 1;
		else
			p_file << 0;

		p_file << endl;
	}
}


void gen_vector(ofstream& p_file, size_t const p_filas)
{
	for(size_t j = 0; j < p_filas-1; j++)
	{
		p_file << 0 << " ";
	}
	p_file << 0 << endl;
}