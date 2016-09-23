#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <climits>

#include "class_Exception.hpp"
#include "class_Big_int.hpp"
#include "class_Rational_number.hpp"
#include "class_Vector.hpp"
#include "class_Matrix.hpp"


Matrix Matrix::operator = (const Matrix &arg)
{
	if (mat != NULL)
		delete [] mat;

	mat_len = arg.mat_len;

	col_size = arg.col_size;
	row_size = arg.row_size;

	if (arg.mat != NULL)
		mat = new Dictionary_matrix [mat_len];
	else
		mat = NULL;

	for (int i = 0; i < mat_len; i++)
	{
		mat[i].col_position = arg.mat[i].col_position;
		mat[i].row_position = arg.mat[i].row_position;
		mat[i].value = arg.mat[i].value;
	}

	return *this;
}

Matrix Matrix::operator + (const Matrix &arg)const
{
	if (mat_len != arg.mat_len)
		throw Exception ("Sizes of matrixs are not equal.");

	Matrix tmp, res;
	Rational_number num1, num2;
	Coords *mas, *mas1;
	int flag = 0, i = 0, j = 0, k = 0, zeroflag = 0;
	long int len = 0;

	mas = new Coords [mat_len + arg.mat_len];

	for (i = 0; i < mat_len; i++)
	{
		mas[i].col_cord = mat[i].col_position;
		mas[i].row_cord = mat[i].row_position;

		if ((mas[i].col_cord == 0) && (mas[i].row_cord == 0))
			zeroflag = 1;
	}

	for (i = mat_len; i < mat_len + arg.mat_len; i++)
	{
		for (j = 0; j < mat_len; j++)
			if ((mas[j].col_cord == arg.mat[i - mat_len].col_position) && (mas[j].row_cord == arg.mat[i - mat_len].row_position))
				flag = 1;

		if (flag == 1)
		{
			flag = 0;
			continue;
		}

		mas[i].col_cord = arg.mat[i - mat_len].col_position;
		mas[i].row_cord = arg.mat[i - mat_len].row_position;

		if ((mas[i].col_cord == 0) && (mas[i].row_cord == 0))
			zeroflag = 1;
	}

	flag = 0;

	for (i = 0; i < mat_len + arg.mat_len; i++)
		if ((mas[i].col_cord != 0) || (mas[i].row_cord != 0) || ((mas[i].col_cord == 0) && (mas[i].row_cord == 0) && (zeroflag == 1)))
		{
			len++;

			if ((mas[i].col_cord == 0) && (mas[i].row_cord == 0) && (zeroflag == 1))
			{
				flag = 1;
				zeroflag = 0;
			}
		}

	mas1 = new Coords [len];

	j = 0;

	for (i = 0; i < mat_len + arg.mat_len; i++)
		if ((mas[i].col_cord != 0) || (mas[i].row_cord != 0) || ((mas[i].col_cord == 0) && (mas[i].row_cord == 0) && (flag == 1)))
		{
			mas1[j].col_cord = mas[i].col_cord;
			mas1[j].row_cord = mas[i].row_cord;
			j++;

			if ((mas[i].col_cord == 0) && (mas[i].row_cord == 0) && (flag == 1))
				flag = 0;
		}

	if (mas != NULL)
        delete [] mas;

    tmp.col_size = arg.col_size;
    tmp.row_size = arg.row_size;
    tmp.mat_len = len;

    if (tmp.mat != NULL)
    	delete [] tmp.mat;

    tmp.mat = new Dictionary_matrix [len];

    for (i = 0; i < len; i++)
    {
    	for (j = 0; j < mat_len; j++)
    		if ((mat[j].col_position == mas1[i].col_cord) && (mat[j].row_position == mas1[i].row_cord))
    		{
    			num1 = mat[j].value;
    			break;
    		}	

    	for (j = 0; j < mat_len; j++)
    		if ((arg.mat[j].col_position == mas1[i].col_cord) && (arg.mat[j].row_position == mas1[i].row_cord))
    		{
    			num2 = arg.mat[j].value;
    			break;
    		}

    	if ((num1 = num1 + num2) != 0)
    	{
    		tmp.mat[k].value = num1;
    		tmp.mat[k].col_position = mas1[i].col_cord;
    		tmp.mat[k].row_position = mas1[i].row_cord;
    		k++;
    	}

    	num1 = 0;
    	num2 = 0;
    }

    if (mas1 != NULL)
        delete [] mas1;

    res.col_size = arg.col_size;
    res.row_size = arg.row_size;
    res.mat_len = k;

    if (k != 0)
    	res.mat = new Dictionary_matrix [k];

    for (i = 0; i < k; i++)
    {
    	res.mat[i].col_position = tmp.mat[i].col_position;
    	res.mat[i].row_position = tmp.mat[i].row_position;
    	res.mat[i].value = tmp.mat[i].value;
    }

    return res;
}

Matrix Matrix::operator - (const Matrix &arg)const
{
	if (mat_len != arg.mat_len)
		throw Exception ("Sizes of matrixs are not equal.");

	Matrix tmp, res;
	Rational_number num1, num2;
	Coords *mas, *mas1;
	int flag = 0, i = 0, j = 0, k = 0, zeroflag = 0;
	long int len = 0;

	mas = new Coords [mat_len + arg.mat_len];

	for (i = 0; i < mat_len; i++)
	{
		mas[i].col_cord = mat[i].col_position;
		mas[i].row_cord = mat[i].row_position;

		if ((mas[i].col_cord == 0) && (mas[i].row_cord == 0))
			zeroflag = 1;
	}

	for (i = mat_len; i < mat_len + arg.mat_len; i++)
	{
		for (j = 0; j < mat_len; j++)
			if ((mas[j].col_cord == arg.mat[i - mat_len].col_position) && (mas[j].row_cord == arg.mat[i - mat_len].row_position))
				flag = 1;

		if (flag == 1)
		{
			flag = 0;
			continue;
		}

		mas[i].col_cord = arg.mat[i - mat_len].col_position;
		mas[i].row_cord = arg.mat[i - mat_len].row_position;

		if ((mas[i].col_cord == 0) && (mas[i].row_cord == 0))
			zeroflag = 1;
	}

	flag = 0;

	for (i = 0; i < mat_len + arg.mat_len; i++)
		if ((mas[i].col_cord != 0) || (mas[i].row_cord != 0) || ((mas[i].col_cord == 0) && (mas[i].row_cord == 0) && (zeroflag == 1)))
		{
			len++;

			if ((mas[i].col_cord == 0) && (mas[i].row_cord == 0) && (zeroflag == 1))
			{
				flag = 1;
				zeroflag = 0;
			}
		}

	mas1 = new Coords [len];

	j = 0;

	for (i = 0; i < mat_len + arg.mat_len; i++)
		if ((mas[i].col_cord != 0) || (mas[i].row_cord != 0) || ((mas[i].col_cord == 0) && (mas[i].row_cord == 0) && (flag == 1)))
		{
			mas1[j].col_cord = mas[i].col_cord;
			mas1[j].row_cord = mas[i].row_cord;
			j++;

			if ((mas[i].col_cord == 0) && (mas[i].row_cord == 0) && (flag == 1))
				flag = 0;
		}

	if (mas != NULL)
        delete [] mas;

    tmp.col_size = arg.col_size;
    tmp.row_size = arg.row_size;
    tmp.mat_len = len;

    if (tmp.mat != NULL)
    	delete [] tmp.mat;

    tmp.mat = new Dictionary_matrix [len];

    for (i = 0; i < len; i++)
    {
    	for (j = 0; j < mat_len; j++)
    		if ((mat[j].col_position == mas1[i].col_cord) && (mat[j].row_position == mas1[i].row_cord))
    		{
    			num1 = mat[j].value;
    			break;
    		}	

    	for (j = 0; j < mat_len; j++)
    		if ((arg.mat[j].col_position == mas1[i].col_cord) && (arg.mat[j].row_position == mas1[i].row_cord))
    		{
    			num2 = arg.mat[j].value;
    			break;
    		}

    	if ((num1 = num1 - num2) != 0)
    	{
    		tmp.mat[k].value = num1;
    		tmp.mat[k].col_position = mas1[i].col_cord;
    		tmp.mat[k].row_position = mas1[i].row_cord;
    		k++;
    	}

    	num1 = 0;
    	num2 = 0;
    }

    if (mas1 != NULL)
        delete [] mas1;

    res.col_size = arg.col_size;
    res.row_size = arg.row_size;
    res.mat_len = k;

    if (k != 0)
    	res.mat = new Dictionary_matrix [k];

    for (i = 0; i < k; i++)
    {
    	res.mat[i].col_position = tmp.mat[i].col_position;
    	res.mat[i].row_position = tmp.mat[i].row_position;
    	res.mat[i].value = tmp.mat[i].value;
    }

    return res;
}

Matrix Matrix::operator - ()
{
	for (int i = 0; i < mat_len; i++)
		mat[i].value = -1 * mat[i].value;

	return *this;
}

Matrix Matrix::operator * (const Matrix &arg)const
{
	if (col_size != arg.row_size)
		throw Exception_matrix_sizes (row_size, col_size, arg.row_size, arg.col_size);

	Matrix rowt, colt, tmp("1", row_size, arg.col_size), res;
	Rational_number tmp_num, num;
	long int count = 0;

	for (int i = 0; i < tmp.mat_len; i++)
		tmp.mat[i].value = 0;

	for (int i = 1; i < row_size + 1; i++)
	{
		Row_position rp(i);
		rowt = (*this)[rp];

		for (int j = 1; j < arg.col_size + 1; j++)
		{
			Col_position cp(j);
			colt = arg[cp];

			for (int k = 1; k < col_size + 1; k++)
			{
				Coords c1(1,k), c2(k,1);

				tmp_num = tmp_num + (rowt[c1] * colt[c2]);
			}

			if (tmp_num != 0)
			{
				tmp.mat[count].value = tmp_num;
				tmp.mat[count].row_position = i - 1;
				tmp.mat[count].col_position = j - 1;
				count++;
			}

			tmp_num = 0; 
		}
	}

	if (count != 0)
		res.mat = new Dictionary_matrix [count];
	else
		res.mat = NULL;

	for (int i = 0; i < count; i++)
	{
		res.mat[i].value = tmp.mat[i].value;
		res.mat[i].row_position = tmp.mat[i].row_position;
		res.mat[i].col_position = tmp.mat[i].col_position; 
	}

	res.mat_len = count;
	res.row_size = row_size;
	res.col_size = arg.col_size;

	return res;
}

Matrix Matrix::operator * (const Rational_number &arg)const
{
	Matrix res(*this);

	if (arg == 0)
	{
		if (res.mat != NULL)
			delete [] res.mat;

		res.mat = NULL;

		res.mat_len = 0;

		return res;
	}

	for (int i = 0; i < mat_len; i++)
		res.mat[i].value = arg * mat[i].value;

	return res;
}

Matrix operator * (const Rational_number &arg1, const Matrix &arg2)
{
	Matrix res(arg2);

	return (res * arg1);
}

Matrix operator * (const Vector &arg1, const Matrix &arg2)
{
	Matrix arg(arg1);

	return (arg * arg2);
}

Matrix operator * (const Matrix &arg1, const Vector &arg2)
{
	Matrix arg(arg2);

	return (arg1 * arg);
}

Matrix Matrix::operator / (const Rational_number &arg)const
{
	if (arg == 0)
        throw Exception ("Division by zero.");

    Matrix res(*this);

    for (int i = 0; i < mat_len; i++)
		res.mat[i].value = mat[i].value / arg;

	return res;
}

Rational_number Matrix::operator [] (const Coords &arg)const
{
	if ((arg.col_cord > col_size) || (arg.row_cord > row_size) || (arg.col_cord < 1) || (arg.row_cord < 1))
	{
		printf("COORDS\n");
		throw Exception_matrix_border (arg.row_cord, arg.col_cord, row_size, col_size);
	}

	Rational_number num;

	for (int i = 0; i < mat_len; i++)
		if ((mat[i].row_position == arg.row_cord - 1) && (mat[i].col_position == arg.col_cord - 1))
			return mat[i].value;

	return num;
}

Matrix Matrix::operator [] (const Row_position &arg)const
{
	if ((arg.row_pos > row_size) || (arg.row_pos < 1))
	{
		printf("ROW\n");
		throw Exception_matrix_border (arg.row_pos, 1, row_size, col_size);
	}

	Matrix tmp, res;
	int count = 0;

	tmp.mat = new Dictionary_matrix [mat_len];

	for (int i = 0; i < mat_len; i++)
		tmp.mat[i].value = 0;

	for (int i = 0; i < mat_len; i++)
		if (mat[i].row_position == arg.row_pos - 1)
		{
			tmp.mat[i].row_position = 0;
			tmp.mat[i].col_position = mat[i].col_position;
			tmp.mat[i].value = mat[i].value;
		}

	for (int i = 0; i < mat_len; i++)
		if (tmp.mat[i].value != 0)
			count++;

	res.mat_len = count;
	res.col_size = col_size;
	res.row_size = 1;

	if (count != 0)
		res.mat = new Dictionary_matrix [count];
	else
	{
		res.mat = NULL;

		return res;
	}

	count = 0;

	for (int i = 0; i < mat_len; i++)
		if (tmp.mat[i].value != 0)
		{
			res.mat[count].row_position = tmp.mat[i].row_position;
			res.mat[count].col_position = tmp.mat[i].col_position;
			res.mat[count].value = tmp.mat[i].value;
			count++;
		}

	return res;
}

Matrix Matrix::operator [] (const Col_position &arg)const
{
	if ((arg.col_pos > col_size) || (arg.col_pos < 1))
	{
		printf("COL\n");
		throw Exception_matrix_border (1, arg.col_pos, row_size, col_size);
	}

	Matrix tmp, res;
	int count = 0;

	tmp.mat = new Dictionary_matrix [mat_len];

	for (int i = 0; i < mat_len; i++)
		tmp.mat[i].value = 0;

	for (int i = 0; i < mat_len; i++)
		if (mat[i].col_position == arg.col_pos - 1)
		{
			tmp.mat[i].row_position = mat[i].row_position;
			tmp.mat[i].col_position = 0;
			tmp.mat[i].value = mat[i].value;
		}

	for (int i = 0; i < mat_len; i++)
	if (tmp.mat[i].value != 0)
		count++;

	res.mat_len = count;
	res.col_size = 1;
	res.row_size = row_size;

	if (count != 0)
		res.mat = new Dictionary_matrix [count];
	else
	{
		res.mat = NULL;

		return res;
	}

	count = 0;

	for (int i = 0; i < mat_len; i++)
		if (tmp.mat[i].value != 0)
		{
			res.mat[count].row_position = tmp.mat[i].row_position;
			res.mat[count].col_position = tmp.mat[i].col_position;
			res.mat[count].value = tmp.mat[i].value;
			count++;
		}

	return res;
}

Matrix Matrix::operator ~ ()const
{
	long int tmp = 0;
	Matrix res(*this);

	tmp = res.row_size;
	res.row_size = res.col_size;
	res.col_size = tmp;

	for (int i = 0; i < mat_len; i++)
	{
		tmp = res.mat[i].row_position;
		res.mat[i].row_position = res.mat[i].col_position;
		res.mat[i].col_position = tmp;
	}

	return res;
}

Matrix::Matrix()
{
	mat = NULL;

	mat_len = 0;
	col_size = 0;
	row_size = 0;

	return;
}

Matrix::Matrix(const Rational_number &number, const long int row, const long int col)
{
	int k = 0;

	col_size = col;
	row_size = row;

	if (number != 0)
	{
		mat_len = row * col;

		mat = new Dictionary_matrix [mat_len];
	}
	else
	{
		mat_len = 0;

		mat = NULL;

		return;
	}

	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
		{
			mat[k].col_position = j;
			mat[k].row_position = i;
			mat[k].value = number;
			k++;
		}

	return;
}

Matrix::Matrix(const Matrix &arg)
{
	col_size = arg.col_size;
	row_size = arg.row_size;

	mat_len = arg.mat_len;

	mat = new Dictionary_matrix [mat_len];

	for (int i = 0; i < mat_len; i++)
	{
		mat[i].col_position = arg.mat[i].col_position;
		mat[i].row_position = arg.mat[i].row_position;
		mat[i].value = arg.mat[i].value;
	}
}

Matrix::Matrix(const Vector &arg)
{
	long int k = 0;

    row_size = 1;
    col_size = arg.get_vector_len();
    mat_len = arg.get_vec_len();

    mat = new Dictionary_matrix [mat_len];

    for (int i = 1; i < col_size + 1; i++)
        if (arg[i] != 0)
        {
            mat[k].value = arg[i];
            mat[k].row_position = 0;
            mat[k].col_position = i - 1;
            k++;
        }

    return;
}

Matrix::Matrix(const char *file)
{

    FILE *f;
    long int coord1 = 0, coord2 = 0;
    int letter = 0, chsz = 0;;
    char *check = NULL;

    if ((f = fopen(file, "r")) == NULL)
        throw Exception_file_name (file);

    letter = getc(f);

    while ((letter != EOF) && ((letter == '\n') || (letter == '#')))
    {
        if (letter == '#')
        {
            while (letter != '\n')
                letter = getc(f);
            letter = getc(f);
            continue;
        }

        if (letter == '\n')
            letter = getc(f);
    }

    if (letter == EOF)
    {
        printf(">Matrix was not founded in this file.\n");

        mat = NULL;

		mat_len = 0;
		col_size = 0;
		row_size = 0;

        fclose(f);
        return;
    }

    while (letter != ' ')
    {
        check = (char *) realloc (check, ++chsz * sizeof(char));
        check[chsz - 1] = letter;
        if ((letter = getc(f)) == EOF)
        {
            printf(">Matrix was not founded in this file.\n");

            if (check != NULL)
                free(check);
            check = NULL;

            mat = NULL;

			mat_len = 0;
			col_size = 0;
			row_size = 0;
            fclose(f);
            return;
        }
    }
    check = (char *) realloc (check, ++chsz * sizeof(char));
    check[chsz - 1] = '\0';

    if (strcmp(check, "Matrix") == 0)
    {
        fscanf(f, "%li", &row_size);
        fscanf(f, "%li", &col_size);

        mat_len = 0;

        mat = NULL;

        letter = getc(f);
        while (letter != EOF)
        {
            if (letter == '#')
            {
                while (letter != '\n')
                    letter = getc(f);
                letter = getc(f);
                continue;
            }

            coord1 = 0;
            coord2 = 0;
            fscanf(f, "%li", &coord1);
            fscanf(f, "%li", &coord2);

            if (check != NULL)
                free(check);
            check = NULL;
            chsz = 0;

            letter = getc(f);
            if (letter == ' ')
                letter = getc(f);

            if ((coord1 != 0) && (coord2 != 0))
            {
                while ((letter != '\n') && (letter != EOF))
                {
                    check = (char *) realloc (check, ++chsz * sizeof(char));
                    check[chsz - 1] = letter;
                    letter = getc(f);
                }

                check = (char *) realloc (check, ++chsz * sizeof(char));
                check[chsz - 1] = '\0';

                this->push(Rational_number(check), coord1, coord2);

                if (letter == EOF)
                    break;
            }
            else
            {
                while ((letter != '\n') && (letter != EOF))
                    letter = getc(f);
            }
        }
        if (check != NULL)
            free(check);
        check = NULL;
    }
    else
    {
        printf(">Matrix was not founded in this file.\n");

        if (check != NULL)
            free(check);
        check = NULL;

        mat = NULL;

		mat_len = 0;
		col_size = 0;
		row_size = 0;
        fclose(f);
        return;
    }

    fclose(f);

    return;
}

Matrix::~Matrix()
{
	if (mat != NULL)
		delete [] mat;

	col_size = 0;
	row_size = 0;
	mat_len = 0;

	return;
}

void Matrix::show(FILE *f)const
{
	if (mat == NULL)
	{
		fprintf(f, "Empty matrix.\n");
		return;
	}

	for (int i = 0; i < mat_len; i++)
	{
		fprintf(f, "%li %li   ",mat[i].row_position + 1, mat[i].col_position + 1);
		mat[i].value.show(f);
	}

	fprintf(f, "\n");
}

void Matrix::push(const Rational_number &number, const long int row, const long int col)
{
    if ((col > col_size) || (row > row_size) || (col < 1) || (row < 1))
		throw Exception_matrix_border (row, col, row_size, col_size);

    if (number == 0)
    {
        for (int i = 0; i < mat_len; i++)
            if ((mat[i].row_position == row - 1) && (mat[i].col_position == col - 1))
            {
                for (int j = i; j < mat_len - 1; j++)
                {
                    mat[j].row_position = mat[j + 1].row_position;
                    mat[j].col_position = mat[j + 1].col_position;
                    mat[j].value = mat[j + 1].value;
                }

                mat_len--;

                if (mat_len == 0)
                {
                    delete [] mat;
                    mat = NULL;
                }
            }

        return;
    }

    for (int i = 0; i < mat_len; i++)
        if ((mat[i].row_position == row - 1) && (mat[i].col_position == col - 1))
        {
            mat[i].value = number;
            return;
        }

    Matrix res;

    res.row_size = row_size;
    res.col_size = col_size;
    res.mat_len = mat_len + 1;

    res.mat = new Dictionary_matrix [res.mat_len];

    for (int i = 0; i < mat_len; i++)
    {
        res.mat[i].row_position = mat[i].row_position;
        res.mat[i].col_position = mat[i].col_position;
        res.mat[i].value = mat[i].value;
    }

    res.mat[res.mat_len - 1].row_position = row - 1;
    res.mat[res.mat_len - 1].col_position = col - 1;
    res.mat[res.mat_len - 1].value = number;

    *this = res;

    return;
}

void Matrix::write(const char *file)
{
	FILE *f;

    if ((f = fopen(file, "w+")) == NULL)
        throw Exception_file_name (file);

    fprintf(f, "Matrix %li %li\n", row_size, col_size);

    for (int i = 0; i < mat_len; i++)
    {
        fprintf(f, "%li %li ",mat[i].row_position + 1, mat[i].col_position + 1);
        mat[i].value.show(f);
    }

    fclose(f);

    return;
}