/**
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: SOFTWARE ENGINEERING LAB. (UG2, Sem2)
 * ASSIGNMENT: TRANSPORTATION PROBLEM USING VOGEL's APPROXIMATION METHOD (QUESTION 1)
 */

/************************************************************************
* COMPILATION: g++ 1.cpp -o 1
* EXECUTION: ./1.exe
------------------------------------------------------------------------
* Sample input for testing code: 

Enter dimensions of allotment matrix:
3 4

Enter allotment matrix:
19 30 50 10
70 30 40 60
40 8 70 20

Enter supplies:
7 9 18

Enter demands:
5 8 7 14
-------------------------------------------------------------------------
* Final Output: [program shows outputs for every iteration]

Final Optimized cost = 779
-------------------------------------------------------------------------
************************************************************************/

#include <bits/stdc++.h>
using namespace std;

/* type definitions to shorten syntax */
typedef pair<int, int> pi;
typedef pair<pair<int, int>, int> ppi;
typedef vector<int> vi;
typedef vector<vector<int>> vvi;

/* macros for ease of syntax */
#define pb push_back
#define index first
#define axis second
#define mp make_pair

/* function to find sum of a vector */
int sumOfVector(vi vec)
{
    int sum = 0;
    for (int i = 0; i < vec.size(); i++)
        sum += vec[i];
    return sum;
}

/* function to get index of minimum value of a vector */
int getMinOfVector(vi vec)
{
    int min = INT_MAX;
    int p_min = 0;
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i] < min)
        {
            min = vec[i];
            p_min = i;
        }
    }
    return p_min;
}

/* utility function to get penalty for a given row or column */
int getPenalty(vi arr)
{
    if (arr.size() == 0)
        return 0;
    else if (arr.size() == 1)
        return arr[0];
    else if (arr.size() == 2)
        return abs(arr[0] - arr[1]);
    int min = INT_MAX, p_min = 0, min2 = INT_MAX;
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i] < min)
        {
            min = arr[i];
            p_min = i;
        }
    }
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i] < min2 && i != p_min)
            min2 = arr[i];
    }
    return abs(min2 - min);
}

/* utility function to get maximum penalty and its index out of row and column penalties */
pi maxPenalty(vi rowpen, vi colpen)
{
    int n_rows = rowpen.size(), n_cols = colpen.size();
    int maxRow = 0, p_row = 0, maxCol = 0, p_col = 0;
    for (int i = 0; i < n_rows; i++)
    {
        if (rowpen[i] > maxRow)
        {
            maxRow = rowpen[i];
            p_row = i;
        }
    }
    for (int i = 0; i < n_cols; i++)
    {
        if (colpen[i] > maxCol)
        {
            maxCol = colpen[i];
            p_col = i;
        }
    }
    if (maxRow > maxCol) // penalty is greater in row
    {
        pi max_pen = mp(p_row, 0); // axis=0 means row
        return max_pen;
    }
    else // penalty is greater in column
    {
        pi max_pen = mp(p_col, 1); // axis=1 means column
        return max_pen;
    }
}

/* removing a row or column of given index from a 2D matrix */
vvi removingRowOrCol(vvi mat, int idx, int axis)
{
    int n_rows = mat.size(), n_cols = mat[0].size();
    if (axis == 0) // row
    {
        vvi temp(n_rows - 1, vi(n_cols));
        for (int i = 0, k = 0; i < n_rows; i++)
        {
            for (int j = 0; j < n_cols; j++)
            {
                if (i != idx)
                    temp[k][j] = mat[i][j];
            }
            if (i != idx)
                k++;
        }
        return temp;
    }
    else // column
    {
        vvi temp(n_rows, vi(n_cols - 1));
        for (int i = 0; i < n_rows; i++)
        {
            for (int j = 0, k = 0; j < n_cols; j++)
            {
                if (j != idx)
                    temp[i][k++] = mat[i][j];
            }
        }
        return temp;
    }
}

/* function to remove a cell of given index from a vector */
vi removeIndexOfVector(vi arr, int idx)
{
    vi temp(arr.size() - 1);
    for (int i = 0, j = 0; i < arr.size(); i++)
    {
        if (i != idx)
            temp[j++] = arr[i];
    }
    return temp;
}

/* add a row or column to a 2D matrix , used for preprocessing unbalanced TP */
vvi addingRowOrCol(vvi mat, vi vec, int axis)
{
    // adding a row or column to allotment matrix for preprocessing step
    int n_row = mat.size();
    if (axis == 0) // row
    {
        mat.push_back(vec);
    }
    else // column
    {
        for (int i = 0; i < n_row; i++)
        {
            mat[i].push_back(vec[i]);
        }
    }
    return mat;
}

/* utility function to compute cost obtained at a given time */
int computeCost(vector<pi> sol)
{
    int cost = 0;
    for (auto pp : sol)
    {
        cost += pp.first * pp.second;
    }
    return cost;
}

/* utility function to copy contents of a matrix into another */
vvi deepcopy(vvi mat2)
{
    int n_rows = mat2.size(), n_cols = mat2[0].size();
    vvi mat(n_rows, vi(n_cols));
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < n_cols; j++)
        {
            mat[i][j] = mat2[i][j];
        }
    }
    return mat;
}

/* utility function to copy contents of a vector into another */
vi deepcopy(vi vec2)
{
    int dim = vec2.size();
    vi vec(dim);
    for (int i = 0; i < dim; i++)
        vec[i] = vec2[i];
    return vec;
}

/* function to print a 2D matrix, used in every iteration */
void DEBUG(vvi mat)
{
    for (int i = 0; i < mat.size(); i++)
    {
        for (int j = 0; j < mat[0].size(); j++)
            cout << mat[i][j] << " ";
        cout << "\n";
    }
}

/* function to print a vector, used in every iteration */
void DEBUG(vi v)
{
    for (auto x : v)
        cout << x << " ";
    cout << "\n";
}

int main()
{
    // variables to store dimensions of allotment matrix
    int n_Rows, n_Cols;
    cout << "Enter dimensions of allotment matrix:\n";
    cin >> n_Rows >> n_Cols;

    // declaring required matrices and arrays
    vvi allot(n_Rows, vi(n_Cols));
    vi supply(n_Rows), demand(n_Cols);

    // taking allotment matrix input
    cout << "\nEnter allotment matrix:\n";
    for (int i = 0; i < n_Rows; i++)
    {
        for (int j = 0; j < n_Cols; j++)
        {
            cin >> allot[i][j];
        }
    }

    // taking supplies as input
    cout << "\nEnter supplies:\n";
    for (int i = 0; i < n_Rows; i++)
    {
        cin >> supply[i];
    }

    // taking demands as input
    cout << "\nEnter demands:\n";
    for (int j = 0; j < n_Cols; j++)
    {
        cin >> demand[j];
    }

    cout << "-------------------------------------------\n";

    /* preprocessing for unbalanced TP */

    int init_supply_sum = sumOfVector(supply), init_demand_sum = sumOfVector(demand);

    if (init_demand_sum > init_supply_sum)
    {
        cout << "Total demand > Total supply; so Unbalanced TP\n";

        // create dummy supply with value equal to difference in sums
        int diff = init_demand_sum - init_supply_sum;
        supply.push_back(diff);

        // add a row to allotment matrix with all zeros
        vi zero(n_Cols, 0);
        allot = addingRowOrCol(allot, zero, 0);
    }
    else if (init_supply_sum > init_demand_sum)
    {
        cout << "Total supply > Total demand; so Unbalanced TP\n";

        // create dummy demand with value equal to difference in sums
        int diff = init_supply_sum - init_demand_sum;
        demand.push_back(diff);

        // add a column to allotment matrix with all zeros
        vi zero(n_Rows, 0);
        allot = addingRowOrCol(allot, zero, 1);
    }

    cout << "\nSupplies after (or without) preprocessing: ";
    DEBUG(supply);
    cout << "Demands after (or without) preprocessing: ";
    DEBUG(demand);

    // updating no. of rows and columns
    n_Rows = allot.size();
    n_Cols = allot[0].size();
    cout << "Dimensions of Allotment matrix after (or without) preprocessing: " << n_Rows << " x " << n_Cols << "\n";

    // declaring arrays for storing row and column penalties
    vi rowpen(n_Rows), colpen(n_Cols);
    for (int i = 0; i < n_Rows; i++)
    {
        rowpen[i] = getPenalty(allot[i]); // getting penalty for each row
    }
    for (int j = 0; j < n_Cols; j++)
    {
        vi vec(n_Rows);
        for (int i = 0; i < n_Rows; i++)
        {
            vec[i] = allot[i][j];
        }
        colpen[j] = getPenalty(vec); // getting penalty for each column
    }

    vector<pi> sol; // vector of pairs for every allocation

    cout << "Initial Column penalties ";
    DEBUG(colpen);
    cout << "Initial Row penalties: ";
    DEBUG(rowpen);

    /* Vogel's Approximation Method algorithm begins here */
    int itr = 1;
    while (sumOfVector(demand) + sumOfVector(supply) > 0)
    {
        cout << "\n\n------------------------------------\n";
        cout << "Iteration " << itr << "\n";
        cout << "-------------------------------------\n";

        // print allotment matrix state at start of iteration
        cout << "Initial allotment matrix for this iteration:\n";
        DEBUG(allot);

        int n_r = allot.size(), n_c = allot[0].size();

        // getting maximum penalty and its location
        pi max_pen = maxPenalty(rowpen, colpen);
        cout << "max penalty at index " << max_pen.index << " Axis = " << max_pen.axis << "\n";

        if (max_pen.axis == 0) // row
        {
            // getting index of minimum element of the row
            int p_min = getMinOfVector(allot[max_pen.index]);
            cout << "Min is at index " << p_min << "\n";

            int coeff = allot[max_pen.index][p_min];
            int res = min(demand[p_min], supply[max_pen.index]);

            // print allotments
            cout << "Allotment: " << coeff << " * " << res << "\n";

            // getting which row or column is to be cancelled from the allotment matrix
            pi cancel = (res != demand[p_min]) ? mp(max_pen.index, 0) : mp(p_min, 1);
            cout << "To be cancelled: index=" << cancel.index << " axis=" << cancel.axis << "\n";

            if (cancel.axis == 0) // remove that row
            {
                vvi new_allot = removingRowOrCol(allot, cancel.index, cancel.axis);
                cout << "New allotment matrix:\n";
                DEBUG(new_allot);

                // update supply and demand
                supply = removeIndexOfVector(supply, cancel.index);
                demand[p_min] -= res;

                // update penalties
                int n_row = new_allot.size(), n_col = new_allot[0].size();
                for (int j = 0; j < n_col; j++)
                {
                    vi vec(n_row);
                    for (int i = 0; i < n_row; i++)
                    {
                        vec[i] = new_allot[i][j];
                    }
                    colpen[j] = getPenalty(vec);
                }
                for (int i = 0; i < n_row; i++)
                {
                    rowpen[i] = getPenalty(new_allot[i]);
                }
                allot = deepcopy(new_allot);
            }
            else // remove that column
            {
                vvi new_allot = removingRowOrCol(allot, cancel.index, cancel.axis);
                cout << "New allotment matrix:\n";
                DEBUG(new_allot);

                // update supply and demands
                demand = removeIndexOfVector(demand, cancel.index);
                supply[max_pen.index] -= res;

                // update penalties
                int n_row = new_allot.size(), n_col = new_allot[0].size();
                for (int i = 0; i < n_row; i++)
                {
                    rowpen[i] = getPenalty(new_allot[i]);
                }
                for (int j = 0; j < new_allot[0].size(); j++)
                {
                    vi vec(n_row);
                    for (int i = 0; i < n_row; i++)
                    {
                        vec[i] = new_allot[i][j];
                    }
                    colpen[j] = getPenalty(vec);
                }
                allot = deepcopy(new_allot);
            }

            sol.pb(mp(coeff, res));

            // remove the row penalty used
            rowpen = removeIndexOfVector(rowpen, max_pen.index);

            cout << "New Supplies: ";
            DEBUG(supply);
            cout << "New Demands: ";
            DEBUG(demand);

            cout << "Updated row penalties: ";
            DEBUG(rowpen);
            cout << "Updated column penalties: ";
            DEBUG(colpen);
        }
        else // column
        {
            // get the column
            vi vec(n_r);
            for (int i = 0; i < n_r; i++)
            {
                vec[i] = allot[i][max_pen.index];
            }

            // get location of min element of column
            int p_min = getMinOfVector(vec);
            cout << "Min is at index " << p_min << "\n";

            int coeff = allot[p_min][max_pen.index];
            int res = min(supply[p_min], demand[max_pen.index]);

            // print allotments
            cout << "Allotment: " << coeff << " * " << res << "\n";

            // get location to be cancelled in allotment matrix
            pi cancel = (res == supply[p_min]) ? mp(p_min, 0) : mp(max_pen.index, 1);
            cout << "To be cancelled: index=" << cancel.index << " axis=" << cancel.axis << "\n";

            if (cancel.axis == 0) // remove that row
            {
                vvi new_allot = removingRowOrCol(allot, cancel.index, cancel.axis);
                cout << "New allotment matrix:\n";
                DEBUG(new_allot);

                // update supply and demand
                supply = removeIndexOfVector(supply, cancel.index);
                demand[max_pen.index] -= res;

                // update penalties
                int n_row = new_allot.size(), n_col = new_allot[0].size();
                for (int i = 0; i < n_row; i++)
                {
                    rowpen[i] = getPenalty(new_allot[i]);
                }
                for (int j = 0; j < new_allot[0].size(); j++)
                {
                    vi vec(n_row);
                    for (int i = 0; i < n_row; i++)
                    {
                        vec[i] = new_allot[i][j];
                    }
                    colpen[j] = getPenalty(vec);
                }
                allot = deepcopy(new_allot);
            }
            else // remove that column
            {
                vvi new_allot = removingRowOrCol(allot, cancel.index, cancel.axis);
                cout << "New allotment matrix:\n";
                DEBUG(new_allot);

                // update supply and demands
                demand = removeIndexOfVector(demand, cancel.index);
                supply[p_min] -= res;

                // update penalties
                int n_row = new_allot.size(), n_col = new_allot[0].size();
                for (int i = 0; i < n_row; i++)
                {
                    rowpen[i] = getPenalty(new_allot[i]);
                }
                for (int j = 0; j < new_allot[0].size(); j++)
                {
                    vi vec(n_row);
                    for (int i = 0; i < n_row; i++)
                    {
                        vec[i] = new_allot[i][j];
                    }
                    colpen[j] = getPenalty(vec);
                }
                allot = deepcopy(new_allot);
            }

            sol.pb(mp(coeff, res));

            // remove the penalty used from column penalties
            colpen = removeIndexOfVector(colpen, max_pen.index);

            cout << "New Supplies: ";
            DEBUG(supply);
            cout << "New Demands: ";
            DEBUG(demand);

            cout << "Updated row penalties: ";
            DEBUG(rowpen);
            cout << "Updated column penalties: ";
            DEBUG(colpen);
        }
        ++itr;
        // print cost obtained so far
        cout << "======> Cost obtained so far = " << computeCost(sol) << "\n";
    }
    /* final output after optimization */
    cout << "\nFinal Optimized cost = " << computeCost(sol) << "\n";
    return 0;
}
