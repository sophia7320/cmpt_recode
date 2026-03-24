from sys import stdin

def _():
    arr1 = [int(x) for x in stdin.readline().split()]
    arr1 = sorted(arr1[1:])
    arr2 = [int(x) for x in stdin.readline().split()]
    arr2 = sorted(arr2[1:])

    z = tuple(zip(arr1,arr2))
    
    ans = sum( x[0]*x[1] for x in z if x[0]*x[1]>0)
    print(ans)

if __name__=="__main__":
    _()