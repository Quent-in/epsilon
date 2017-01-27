extern "C" {
#include <assert.h>
#include <stdlib.h>
}

#include <poincare/multiplication.h>
#include "layout/string_layout.h"
#include "layout/horizontal_layout.h"
#include "layout/parenthesis_layout.h"

Expression::Type Multiplication::type() const {
  return Expression::Type::Multiplication;
}

ExpressionLayout * Multiplication::createLayout() const {
  ExpressionLayout** children_layouts = (ExpressionLayout **)malloc(3*sizeof(ExpressionLayout *));
  children_layouts[0] = m_operands[0]->createLayout();
  children_layouts[1] = new StringLayout("*", 1);
  children_layouts[2] = m_operands[1]->type() == Type::Opposite ? new ParenthesisLayout(m_operands[1]->createLayout()) : m_operands[1]->createLayout();
  return new HorizontalLayout(children_layouts, 3);
}

float Multiplication::approximate(Context& context) const {
  return m_operands[0]->approximate(context)*m_operands[1]->approximate(context);;
}

Expression * Multiplication::cloneWithDifferentOperands(Expression** newOperands,
    int numberOfOperands, bool cloneOperands) const {
  assert(numberOfOperands == 2);
  assert(newOperands != nullptr);
  return new Multiplication(newOperands, cloneOperands);
}

Expression * Multiplication::evaluateOnMatrixAndFloat(Matrix * m, Float * a, Context& context) const {
  Expression * operands[m->numberOfRows() * m->numberOfColumns()];
  for (int i = 0; i < m->numberOfRows() * m->numberOfColumns(); i++) {
    operands[i] = new Float(m->operand(i)->approximate(context)*a->approximate(context));
  }
  return new Matrix(operands, m->numberOfRows() * m->numberOfColumns(), m->numberOfColumns(), m->numberOfRows(), false);
}

Expression * Multiplication::evaluateOnFloatAndMatrix(Float * a, Matrix * m, Context& context) const {
  return evaluateOnMatrixAndFloat(m, a, context);
}

Expression * Multiplication::evaluateOnMatrices(Matrix * m, Matrix * n, Context& context) const {
  if (m->numberOfColumns() != n->numberOfRows()) {
    return nullptr;
  }
  Expression * operands[m->numberOfRows() * n->numberOfColumns()];
  for (int i = 0; i < m->numberOfRows(); i++) {
    for (int j = 0; j < n->numberOfColumns(); j++) {
      float f = 0.0f;
      for (int k = 0; k < m->numberOfColumns(); k++) {
        f += m->operand(i*m->numberOfColumns()+k)->approximate(context) * n->operand(k*n->numberOfColumns()+j)->approximate(context);
      }
      operands[i*n->numberOfColumns()+j] = new Float(f);
    }
  }
  return new Matrix(operands, m->numberOfRows() * n->numberOfColumns(), m->numberOfRows(), n->numberOfColumns(), false);
}