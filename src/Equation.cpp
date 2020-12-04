#include "Equation.hpp"
#include "VectorOperations.hpp"

Equation::Equation(const size_t N) : N(N), K(N, N), f(N, 0)
{
    
}

// https://en.wikipedia.org/wiki/Conjugate_gradient_method
// The details of the algorithm are complicated, but the important thing is, that it can solve a quadratic, symmetric and positive definite matrix in a short time.
pair<unique_ptr<vector<float>>, int> Equation::SolveIterative() 
{
    // Pointer swap variables
    unique_ptr<vector<float>> x_k = make_unique<vector<float>>(N, 0);
    unique_ptr<vector<float>> r_k = make_unique<vector<float>>(N, 0);
    unique_ptr<vector<float>> p_k = make_unique<vector<float>>(N, 0);

    unique_ptr<vector<float>> x_k1 = make_unique<vector<float>>(N, 0);
    unique_ptr<vector<float>> r_k1 = make_unique<vector<float>>(N, 0);
    unique_ptr<vector<float>> p_k1 = make_unique<vector<float>>(N, 0);

    // Temporary variables
    vector<float> kTimesP(N, 0);
    vector<float> kTimesx_k(N, 0);
    vector<float> scaledP_K(N, 0);
    vector<float> scaledKTimesP(N, 0);
    float alpha_k = 1;
    float alpha_k_divider = 1;
    float beta_k = 1;

    K.Multiply(*x_k, kTimesx_k);
    subtract(f, kTimesx_k, *r_k);
    *p_k = *r_k;
    size_t counter = 0;
    const size_t maxSteps = 10000;
    #pragma omp parallel
    while (counter < maxSteps)
    {
        {
            fillZeros(kTimesP);
            K.Multiply(*p_k, kTimesP);
        }
        #pragma omp barrier

        #pragma omp single
        {
            alpha_k_divider = 0;
            for (size_t n = 0; n < N; n++)
                alpha_k_divider += (*p_k)[n] * kTimesP[n];
        }
        #pragma omp barrier

        {
            if (alpha_k_divider < 1e-12) // Appears if f = 0
                break;
        }
        #pragma omp barrier

        #pragma omp single
        {
            alpha_k = l2square(*r_k) / alpha_k_divider;
        }
        #pragma omp barrier

        {
            multiply(alpha_k, *p_k, scaledP_K);
            multiply(alpha_k, kTimesP, scaledKTimesP);
            add(*x_k, scaledP_K, *x_k1);
            subtract(*r_k, scaledKTimesP, *r_k1);
        }
        #pragma omp barrier

        if (l2square(*r_k1) < 1e-10)
            break;

        #pragma omp single
        {
            beta_k = l2square(*r_k1) / l2square(*r_k); // TODO: Do not recalculate this
        }
        #pragma omp barrier

        
        {
            multiply(beta_k, *p_k, scaledP_K);
            add(*r_k1, scaledP_K, *p_k1);
        }
        #pragma omp barrier

        #pragma omp single
        {
            x_k.swap(x_k1);
            r_k.swap(r_k1);
            p_k.swap(p_k1);
            counter++;
        }
        #pragma omp barrier
    }
    
    if (counter == maxSteps) {
        cerr << "Warning, EquationSolver did not converge" << endl;
    }

    return pair<unique_ptr<vector<float>>, int>(move(x_k1), counter);
}

ostream& operator<<(ostream& os, const Equation& equation) 
{
    os << equation.K;
    for (float _f: equation.f)
        os << _f << " ";
    return os << endl;
}
