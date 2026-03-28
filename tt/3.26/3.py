from sys import stdin as pin

def _():
    n = int(pin.readline())
    tms = [int(x) for x in pin.readline().split()]

    st , ed = [] , []

    p=0
    t=0
    while p<n:
        
        t=tms[p]+15
        while p<n and tms[p]<t :
            p+=1
        st.append(t)
        
        fg =False
        while p<n and tms[p]<t+30-1:
            p+=1
            fg=True
        
        while fg and p<n and tms[p]<t+45-1:
            p+=1
        
        if fg:
            ed.append(t+45-1)
        else:
            ed.append(t+30-1)
    
    for i in range(len(st)):
        print(st[i],ed[i])

if __name__=="__main__":
    _()
