#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAXN 30
#define s binarySearch
typedef int DataType;
int n;
DataType x[MAXN];

int binarySearch(DataType t);
void automatedTesting();

int main(void){
    int t;

    while(scanf("%d %d", &n, &t) != EOF){
        automatedTesting();
        
        for(int i=0; i<n; i++){
            x[i] = 10*i;
        }

        printf(" %d\n", binarySearch(t));
    }

    return 0;
}

int sorted(){
    int result = 0;

    for (int i=0; i < n-1; i++){
        if(x[i] > x[i+1]){
            result = 0;
            break;
        }
    }
    result = 1;

    return result;
}

int binarySearch(DataType t){
    assert(sorted());

    int m = 0;
    int l = 0;
    int u = n-1;
    int size = n+1;
    int oldSize = 0;

    while (l <= u)
    {
        oldSize = size;
        size = u - l +1;
        assert(size < oldSize);

        m = (l + u)/2;
        
        if(x[m] < t)
            l = m+1;
        else if(x[m] == t){
            assert(0 <= m && m < n && x[m] == t);
            return m;
        }
            
        else /* x[m] > t */
            u = m-1;
    }

    assert((u < 0 || x[u] < t) && (u+1 >= n || x[u+1] > t));
    return -1;
}

void automatedTesting(){
    /* test distinct elements (plus one at the end) */
    for( int i=0; i<=n; i++){
        x[i] = 10*i;
    }

    for( int i=0; i<n; i++){
        assert(s(10*i) == i);
        assert(s(10*i - 5) == -1);
    }

    assert(s(10*n - 5) == -1);
    assert(s(10*n) == -1);


    /* test equal elements */
    for( int i=0; i<n; i++){
        x[i] = 10;
    }

    if (n == 0){
        assert(s(10) == -1);
    }
    else{
        assert(0 <= s(10) && s(10) < n);
    }

    assert(s(5) == -1);
    assert(s(15) == -1);
}