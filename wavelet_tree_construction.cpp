#include<bits/stdc++.h>
using namespace std;

const int MAX = 1e6;
const int N = 3e5;

int g[N];

// Entire Construction Of Wavelet Tree
struct wavelet_Tree{
    int lo;
    int hi;
    wavelet_Tree *l, *r;
    // Frequency Array to store count of the elements that will go
    // to the left and right child of the node
    vector<int> b;

    // Values are in range [x,y]
    // Array indices are [from, to]
    wavelet_Tree(int *from, int *to, int x, int y){
        lo = x;
        hi = y;
        // If lower index value becomes greater than higher index value return
        if(lo == hi || from>=to){
            return;
        }
        // Initialising mid as lo + hi/2
        int mid = lo + (hi-lo)/2;

        // lamda function to return boolean value true when value is less than mid 
        auto f = [mid](int x){
            return x<= mid;
        };
        // Reserving space in vector from starting index to ending index
        b.reserve(to - from + 1);
        // Pushing 0 value for initialisation 
        b.push_back(0);

        for(auto it = from; it!=to; it++){
            b.push_back(b.back() + f(*it));
        }
        // Using stable partition to arrange the vector elements according to boolean function f
        auto pivot = stable_partition(from, to, f);

        l = new wavelet_Tree(from, pivot, lo, mid);
        r = new wavelet_Tree(pivot, to, mid+1, hi);
    }

    // Range query - 1
    // To find the kth smallest number in [l,r]

    int kth(int l, int r, int k){
        if(l>r){
            return 0;
        }
        if(lo==hi){
            return lo;
        }
        int inLeft = b[r] - b[l-1];
        int lb = b[l-1]; //Amount of no.s in first l elements that go to left child
        int rb = b[r];  //Amount of no.s till first r elements that go to left child
        if(k<=inLeft){
            return this->l->kth(lb+1, rb, k);
        }
        return this->r->kth(l-lb, r-rb, k-inLeft);
    }

    // Range Query - 2
    // Count of numbers less than equal to k in [l,r]

    int ltk(int l, int r, int k){
        if(l>r){
            return 0;
        }
        if(lo==hi){
            return r-l+1;
        }
        int lb = b[l-1];
        int rb = b[r];

        return this->l->ltk(lb+1, rb, k) + this->r->ltk(l-lb, r-rb, k);
    }

    // Query - 3
    // Count of numbers in [l,r] equal to k

    int count(int l, int r, int k){
        if(l>r || k<lo || k>hi){
            return 0;
        }
        if(lo==hi){
            return r-l+1;
        }
        int lb = b[l-1];
        int rb = b[r];
        int mid = (lo + hi)/2;
        if(k<=mid){
            return this->l->count(lb+1, rb, k);
        }
        return this->r->count(l-lb, r-rb, k);
    }

    // destructor
    ~wavelet_Tree(){
        delete l;
        delete r;
    }

};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    srand(time(NULL));
    int n;
    cin>>n;        

    for(int i=0; i<n; i++){
        cin>>g[i+1];
    }

    wavelet_Tree t(g+1, g+n+1, 1, MAX);
    
    int dec;
    cout<<"Choose Operation: 0,1,2"<<endl;
    cin>>dec;
    int l, r, k;
    cin>>l>>r>>k;
    if(dec==0){
        // Kth smallest
        cout<<"Kth smallest: ";
        cout<<t.kth(l, r, k)<<endl;
    }
    else if(dec==1){
        // Count of numbers less than equal to k
        cout<<"Count of numbers less than equal to k: ";
        cout<<t.ltk(l,r,k)<<endl;
    }
    else if(dec==2){
        // Count of numbers in [l,r] equal to k
        cout<<"Count of numbers in range equal to k: ";
        cout<<t.count(l,r,k)<<endl;
    }

}

