#include "Equation.hpp"

Equation::Equation(const int N) : N(N), K(N, N, 0), f(N, 0)
{
    
}

// https://en.wikipedia.org/wiki/Conjugate_gradient_method
unique_ptr<vector<float>> Equation::SolveIterative() 
{
    unique_ptr<vector<float>> x_k = make_unique<vector<float>>(N, 0);
    unique_ptr<vector<float>> r_k = make_unique<vector<float>>(N, 0);
    unique_ptr<vector<float>> p_k = make_unique<vector<float>>(N, 0);

    unique_ptr<vector<float>> x_k1 = make_unique<vector<float>>(N, 0);
    unique_ptr<vector<float>> r_k1 = make_unique<vector<float>>(N, 0);
    unique_ptr<vector<float>> p_k1 = make_unique<vector<float>>(N, 0);

    *r_k = subtract(f, K * *x_k);
    *p_k = *r_k;
    int counter = 0;
    while (counter < 50)
    {
        vector<float> kTimesP = K * *p_k;

        float alpha_k_divider = 0;
        for (int n = 0; n < N; n++)
            alpha_k_divider += (*p_k)[n] * kTimesP[n];
        float alpha_k = l2square(*r_k) / alpha_k_divider;

        vector<float> scaledP_K = multiply(alpha_k, *p_k);
        vector<float> scaledKTimesP = multiply(alpha_k, kTimesP);
        *x_k1 = add(*x_k, scaledP_K);
        *r_k1 = subtract(*r_k, scaledKTimesP);
        if (l2square(*r_k1) < 1e-10)
        {
            return x_k1;
        }
        
        float beta_k = l2square(*r_k1) / l2square(*r_k);
        scaledP_K = multiply(beta_k, *p_k);
        *p_k1 = add(*r_k1, scaledP_K);

        x_k.swap(x_k1);
        r_k.swap(r_k1);
        p_k.swap(p_k1);
        counter++;
    }
    
    cerr << "Warning, EquationSolver did not converge" << endl;

    return x_k1;
}

void Equation::Print() 
{
    K.Print();
    for (float _f: f)
        cout << _f << endl;
}