// AUTOR: 
// FECHA: 
// EMAIL: 
// VERSION: 2.0
// ASIGNATURA: Algoritmos y Estructuras de Datos
// PRÁCTICA Nº: 4
// ESTILO: Google C++ Style Guide
// COMENTARIOS:
// 

#ifndef SLLPOLYNOMIAL_H_
#define SLLPOLYNOMIAL_H_

#include <iostream>
#include <math.h>  // fabs, pow

#include "pair_t.h"
#include "sll_t.h"
#include "vector_t.h"

#define EPS 1.0e-6

typedef pair_t<double> pair_double_t;  // Campo data_ de SllPolynomial
typedef sll_node_t<pair_double_t> SllPolyNode;  // Nodos de SllPolynomial

// Clase para polinomios basados en listas simples de pares
class SllPolynomial : public sll_t<pair_double_t> {
 public:
  // constructores
  SllPolynomial(void) : sll_t() {};
  SllPolynomial(const vector_t<double>&, const double = EPS);

  // destructor
  ~SllPolynomial() {};

  // E/S
  void Write(std::ostream& = std::cout) const;
  
  // operaciones
  double Eval(const double) const;
  bool IsEqual(const SllPolynomial&, const double = EPS) const;
  void Sum(const SllPolynomial&, SllPolynomial&, const double = EPS);
};


bool IsNotZero(const double val, const double eps = EPS) {
  return fabs(val) > eps;
}

// FASE II
// constructor

SllPolynomial::SllPolynomial(const vector_t<double>& v, const double eps) { //se crea la lista, recorro el vector y almaceno las parejas por el frente
  for (int i{v.get_size() - 1}; i >= 0; i--) {
    if (IsNotZero(v.at(i), eps)) {
      pair_double_t pair(v.get_val(i), i);
      push_front(new SllPolyNode(pair));  
    }
  }
}


// E/S
void SllPolynomial::Write(std::ostream& os) const {
  os << "[ ";
  bool first{true};
  SllPolyNode* aux{get_head()};
  while (aux != NULL) {
    int inx{aux->get_data().get_inx()};
    double val{aux->get_data().get_val()};
    if (val > 0)
      os << (!first ? " + " : "") << val;
    else
      os << (!first ? " - " : "-") << fabs(val);
    os << (inx > 1 ? " x^" : (inx == 1) ? " x" : "");
    if (inx > 1)
      os << inx;
    first = false;
    aux = aux->get_next();
  }
  os << " ]" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const SllPolynomial& p) {
  p.Write(os);
  return os;
}


// Operaciones con polinomios

// FASE III
// Evaluación de un polinomio representado por lista simple
double SllPolynomial::Eval(const double x) const {
  double result{0.0};
  SllPolyNode* aux = get_head();
  while (aux != NULL) {
    result += pow(x, aux->get_data().get_inx()) * aux->get_data().get_val();
    aux = aux->get_next();
  }
  return result;
}

// Comparación si son iguales dos polinomios representados por listas simples
bool SllPolynomial::IsEqual(const SllPolynomial& sllpol, const double eps) const {
  bool differents = false;
  SllPolyNode* aux1 = get_head();
  SllPolyNode* aux2 = sllpol.get_head();
  while (aux1 != NULL && aux2 != NULL)  {
    if (fabs(aux1->get_data().get_val() - aux2->get_data().get_val() > eps) || fabs(aux1->get_data().get_inx() - aux2->get_data().get_inx() > eps)) {
      differents = true;
      return !differents;
    }
    aux1 = aux1->get_next();
    aux2 = aux2->get_next();
  }

  if (aux1 != NULL || aux2 != NULL) {
    differents = true;
    return !differents;
  }

  return !differents;
}

// FASE IV
// Generar nuevo polinomio suma del polinomio invocante mas otro polinomio
void SllPolynomial::Sum(const SllPolynomial& sllpol, SllPolynomial& sllpolsum, const double eps) {
  SllPolyNode* aux1 = get_head();
  SllPolyNode* aux2 = sllpol.get_head();
  SllPolynomial sllpolsum_aux;
  while (aux1 != NULL && aux2 != NULL) {
    if (aux1->get_data().get_inx() - aux2->get_data().get_inx() > eps) { //si el índice del monomio del primero polinomio es mayor que el del primero
      sllpolsum_aux.push_front(new SllPolyNode(aux2->get_data()));
      aux2 = aux2->get_next();
    } else if (aux2->get_data().get_inx() - aux1->get_data().get_inx() > eps) { //si el índice del monomio del segundo polinomo es mayor que el del primero
      sllpolsum_aux.push_front(new SllPolyNode(aux1->get_data()));
      aux1 = aux1->get_next();
    } else { //el último cado que queda es que ambos índices sean iguales
      pair_double_t pair(aux1->get_data().get_val() + aux2->get_data().get_val(), aux1->get_data().get_inx());
      if (IsNotZero(pair.get_val(), eps)) {
        sllpolsum_aux.push_front(new SllPolyNode(pair));
      }
      aux1 = aux1->get_next();
      aux2 = aux2->get_next();
    }
  }
  //Todo este código si resulta que ambos polinomios tienen el mayor índice igual, pero y si el de uno fuera mayor
  
  while (aux1 != NULL || aux2 != NULL) {
    if (aux1 != NULL) {
      sllpolsum_aux.push_front(new SllPolyNode(aux1->get_data()));
      aux1 = aux1->get_next();
    } else if (aux2 != NULL) {
      sllpolsum_aux.push_front(new SllPolyNode(aux2->get_data()));
      aux2 = aux2->get_next();
    }
  }
  
  SllPolyNode* aux4 = sllpolsum_aux.get_head();
  while (aux4 != NULL) {
    sllpolsum.push_front(new SllPolyNode(aux4->get_data()));
    aux4 = aux4->get_next();
  }
}


#endif  // SLLPOLYNOMIAL_H_
