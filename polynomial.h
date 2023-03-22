// AUTOR: Samuel Martín Morales
// FECHA: 15-03-2023
// EMAIL: alu0101359526@ull.edu.es
// VERSION: 1.0
// ASIGNATURA: Algoritmos y Estructuras de Datos
// PRÁCTICA Nº: 3
// ESTILO: Google C++ Style Guide
// COMENTARIOS:
// 

// Path: polynomial.h
#ifndef POLYNOMIAL_H_
#define POLYNOMIAL_H_

// Librerías necesarias
#include <iostream>
#include <math.h>  // fabs, pow

// Classes necesarias
#include "vector_t.h"
#include "sparse_vector_t.h"

// Clase para polinomios basados en vectores densos de doubles
class Polynomial : public vector_t<double> {
 public:
  // constructores
  Polynomial(const int n = 0) : vector_t<double>(n) {};
  Polynomial(const Polynomial& pol)
      : vector_t<double>(pol) {}; // constructor de copia

  // destructor
  ~Polynomial() {};

  // E/S
  void Write(std::ostream& = std::cout, const double eps = EPS) const;
  
  // operaciones
  double Eval(const double) const;
  bool IsEqual(const Polynomial&, const double = EPS) const;
 };


// Clase para polinomios basados en vectores dispersos
class SparsePolynomial : public sparse_vector_t {
 public:
  // constructores
  SparsePolynomial(const int n = 0) : sparse_vector_t(n) {};
  SparsePolynomial(const Polynomial& pol) : sparse_vector_t(pol) {};
  SparsePolynomial(const SparsePolynomial&);  // constructor de copia

  // destructor
  ~SparsePolynomial() {};

  // E/S
  void Write(std::ostream& = std::cout) const;
  
  // operaciones
  double Eval(const double) const;
  bool IsEqual(const SparsePolynomial&, const double = EPS) const;
  bool IsEqual(const Polynomial&, const double = EPS) const;

  // Modificación de la clase sparse_polynomial_t
  int monomial_grade_add() const;
};

// E/S
void Polynomial::Write(std::ostream& os, const double eps) const {
  os << get_size() << ": [ ";
  bool first{true};
  for (int i{0}; i < get_size(); i++)
    if (IsNotZero(at(i), eps)) {
      os << (!first ? " + " : "") << at(i)
	 << (i > 1 ? " x^" : (i == 1) ? " x" : "");
      if (i > 1)
	os << i;
      first = false;
    }
  os << " ]" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
  p.Write(os);
  return os;
}

// Operaciones con polinomios

// Evaluación de un polinomio representado por vector denso
double Polynomial::Eval(const double x) const {
  double result{0.0};
  for (int i{0}; i < get_size(); i++)
    result += at(i) * pow(x, i);
  return result;
}

// Comparación si son iguales dos polinomios representados por vectores densos
bool Polynomial::IsEqual(const Polynomial& pol, const double eps) const {
  bool differents = false;
  for (int i = 0; i < get_size(); i++) {
      if (IsNotZero(at(i),eps) == true && IsNotZero(pol.at(i), eps) == true) {
        if (at(i) != pol.at(i)) {
          differents = true;
        }
      }
    }
  
  return !differents;
}

// constructor de copia
SparsePolynomial::SparsePolynomial(const SparsePolynomial& spol) {
  *this = spol;   // se invoca directamente al operator=
}

// E/S
void SparsePolynomial::Write(std::ostream& os) const {
  os << get_n() << "(" << get_nz() << "): [ ";
  bool first{true};
  for (int i{0}; i < get_nz(); i++) {
    int inx{at(i).get_inx()};
    os << (!first ? " + " : "") << at(i).get_val()
       << (inx > 1 ? " x^" : (inx == 1) ? " x" : "");
    if (inx > 1)
      os << inx;
    first = false;
  }
  os << " ]" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const SparsePolynomial& p) {
  p.Write(os);
  return os;
}

// Operaciones con polinomios

// Evaluación de un polinomio representado por vector disperso
double SparsePolynomial::Eval(const double x) const {
  double result{0.0};
  for (int i{0}; i < get_nz(); i++)
    result += at(i).get_val() * pow(x, at(i).get_inx());
  return result;
}

// Comparación si son iguales dos polinomios representados por vectores dispersos
bool SparsePolynomial::IsEqual(const SparsePolynomial& spol
			       , const double eps) const {
  bool differents = false;
  if (get_n() != spol.get_n() || get_nz() != spol.get_nz())
    differents = true;
  else
    for (int i{0}; i < get_nz() && !differents; i++)
      differents = IsNotZero(at(i).get_val() - spol.at(i).get_val(), eps);
  return !differents;
}

// Comparación si son iguales dos polinomios representados por
// vector disperso y vector denso
bool SparsePolynomial::IsEqual(const Polynomial& pol, const double eps) const {
  bool differents = false;
  int nz_ = 0;
  for (int i = 0; i < pol.get_size(); i++) {
    if (IsNotZero(pol.at(i), eps) == true) {
      nz_++;
    }
  }

  int counter = 0;
  if (nz_ == get_nz()) {
    for (int i = 0; i < pol.get_size(); i++) {
      if (IsNotZero(pol.at(i), eps) == true) {
        if (pol.at(i) != at(counter).get_val()) {
          differents == true;
        }
        counter++;
      }
    }
  } else if (nz_ != get_nz()) {
    differents = true;
  }
  return !differents;
}

// Implementación de la modificación de la clase sparse_polynomial_t
int SparsePolynomial::monomial_grade_add() const {
  // Suma total de los grados de los monomios
  int sum = 0;
  for (int i = 0; i < get_nz(); i++) {
    if (at(i).get_val() != 0)
      sum += at(i).get_inx();
  }
  return sum;
}


#endif  // POLYNOMIAL_H_
