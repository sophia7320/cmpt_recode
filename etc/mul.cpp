#include <iostream>
#include <vector>
#include <format>

using namespace std;

class mat{
private:
    vector<vector<int>> data;

public:

    mat(int rw,int cl){
        this->col_size=cl;
        this->row_size=rw;
        if (cl>0&&rw>0){
            data=vector<vector<int>>(rw);
            for (auto& row : data){
                row.resize(cl);
            }
        }
    }

    int col_size;
    int row_size;

    vector<int>& operator[](int idx){
        return data[idx];
    }

    mat operator*(mat& other){
        if (col_size != other.row_size){
            throw runtime_error(format("Error: {} != {}",col_size,other.row_size));
        }

        mat res_mat(row_size,other.col_size);

        for (int r_idx=0; r_idx<res_mat.row_size;r_idx++ ){
            for(int c_idx=0; c_idx<res_mat.col_size;c_idx++){
                for(int i=0;i<col_size;i++){
                    res_mat[r_idx][c_idx]+=data[r_idx][i]*other[i][c_idx];
                }
            }
        }

        return move(res_mat);
    }

    void print(){
        for (int i=0;i<row_size;i++){
            for(int j=0;j<col_size;j++){
                cout<<data[i][j];
                if (j==col_size-1){
                    cout<<"\n";
                }else{
                    cout<<" ";
                }
            }
        }
    }
};

mat mat_input(){
    int r,c;
    cin>>r>>c;
    mat res(r,c);
    for(int i=0 ; i<r ; i++){
        for (int j=0 ; j<c ;j++){
            cin>>res[i][j];
        }
    }

    return move(res);
}

int main(){
    mat a=mat_input();
    mat b=mat_input();

    try{
        auto res=a*b;
        cout<<res.row_size<<" "<<res.col_size<<endl;
        res.print();
        return 0;
    }catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return 1;
    }
}