from sys import stdin

def _():
    n = int(stdin.readline())
    ts = [int(x) for x in stdin.readline().split()]

    p=0
    t=0

    st=[]
    ed=[]

    red=False
    scd=False

    down=False

    rt=0
    ct=0
    while True:
        t+=1
        #print(f"t={t} , red={red}")
        if not red:
            if down:
                rt-=1
                #print(rt)
                while p<n and ts[p]==t:
                    p+=1
                if rt==0:
                    ct-=1
                    red=True
                    down=False
                    st.append(t)
            if p<n and ts[p]==t and not down:
                #print(p)
                scd=False
                down=True
                ct=30
                rt=15
                while p<n and ts[p]==t:
                    p+=1
        else:
            ct-=1
            if ct==0:
                #print(9)
                red=False
                down =False
                ed.append(t)
                if p==len(ts):
                    break
                if p<n and ts[p]==t:
                #print(p)
                    scd=False
                    down=True
                    ct=30
                    rt=15
                    while p<n and ts[p]==t:
                        p+=1
            if scd and p<n and ts[p]==t:
                while p<n and ts[p]==t:
                    p+=1

            if not scd and p<n:
                if ts[p] == t:
                    while p<n and ts[p]==t:
                        p+=1
                    scd=True
                    ct+=15

    for i in range(len(st)):
        print(st[i],ed[i])





if __name__ == "__main__":
    _()