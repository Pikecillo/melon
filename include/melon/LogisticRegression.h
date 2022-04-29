#pragma once

#include <melon/LogisticModel.h>
#include <melon/Regression.h>

namespace ml {
template <size_t dim>
class LogisticRegressionCostFunction : public CostFunction<LogisticModel<dim>> {
  public:
    using model_type = LogisticModel<dim>;
    using argument_type = typename model_type::parameters_type;
    using gradient_type = argument_type;
    using training_set_type = TrainingSet<dim>;

  public:
    LogisticRegressionCostFunction(const training_set_type &trainingSet)
        : CostFunction<model_type>(trainingSet) {}

    double eval(const argument_type &input) const {
        model_type model(input);
        model.setParameters(input);

        double cost = 0.0;
        for (const auto &[x, y] : this->m_trainingSet) {
            const double prediction = 1.0 / (1.0 + std::exp(-model.eval(x)));
            cost += (y * log(prediction) + (1.0 - y) * log(1.0 - prediction));
        }

        const double numExamples = static_cast<double>(this->m_trainingSet.size());
        return -cost / numExamples;
    }
};

/**
   Logistic model regression.
 */
template <size_t dim>
class LogisticRegression
    : public Regression<LogisticModel<dim>, LogisticRegressionCostFunction<dim>> {
  public:
    using cost_function_type = LogisticRegressionCostFunction<dim>;
    using training_set_type = typename cost_function_type::training_set_type;

  private:
    virtual LogisticRegressionCostFunction<dim>
    getCostFunction(const training_set_type &trainingSet) {
        return LogisticRegressionCostFunction<dim>(trainingSet);
    }
};
} // namespace ml
