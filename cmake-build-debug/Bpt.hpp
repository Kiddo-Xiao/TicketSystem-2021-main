#ifndef _BPlusTree_h
#define _BPlusTree_h

#include<iostream>
#include<cstdio>
#include<cstring>
#include<fstream>
// test
#include<cassert>
#include<vector>

using namespace std;

template<typename TYPE_KEY, typename TYPE_VAL>
class BPlusTree {
public:
    typedef std::pair<TYPE_KEY, TYPE_VAL> TYPE;
    static const int Block = 30, Lim = Block >> 1, maxn = 1e6 + 5;
    friend bool operator < (const TYPE &a, const TYPE &b) {
        return a.first == b.first ? a.second < b.second : a.first < b.first;
    }
    friend bool operator <= (const TYPE &a, const TYPE &b) {
        return a.first == b.first ? a.second < b.second : a.first <= b.first;
    }
    // === debug === //
    // int cnt_read = 0, cnt_write = 0, cnt_insert = 0, cnt_ins = 0, cnt_shift = 0;
    // double clock_ins = 0, clock_ins1 = 0, clock_ins2 = 0, clock_ins3 = 0, clock_read = 0, clock_write = 0, clock_insert = 0;
    // === debug === //

private:
    struct Node {

        int right = -1, left = -1, size = 0;
        TYPE val[Block + 1];
        int son[Block + 1];
        bool leaf = true;
        Node(int right = - 1, int left = -1, int size = 0, bool leaf = true) : right(right), left(left), size(size), leaf(leaf) {
            memset(son, -1, sizeof son);
        }

        Node &operator = (const Node &other) {
            if(this == &other)
                return *this;

            leaf = other.leaf;
            size = other.size;
            memcpy(son, other.son, sizeof other.son);
            memcpy(val, other.val, sizeof other.val);
            return *this;
        }

        // === debug === //
         void Print() {
         	std :: cout << "Now Node" << std :: endl;
         	std :: cout << "size = " << size << " leaf = " << leaf << " left = " << left << " right = " << right << std :: endl;
//         	for(int i = 0; i < size; ++ i)
//         		std :: cout << "Val[" << i << "] = Pair(" << val[i].first << ", " << val[i].second << ") ";
         	std :: cout << std :: endl;
         	for(int i = 0; i < size; ++ i)
         		std :: cout << "Son[" << i << "] = " << son[i] << " ";
         	std :: cout << std :: endl;
         }
        // === debug === //
    };

    int rec[maxn], top;

    char file[50];
    std :: fstream io;
    template<class T>
    void Read(T &x, int pos) {
        io.seekg(pos, std :: ios :: beg);
        io.read(reinterpret_cast<char *>(&x), sizeof(x));
    }

    template<class T>
    void Write(T &x, int pos) {
        io.seekg(pos, std :: ios :: beg);
        io.write(reinterpret_cast<char *>(&x), sizeof(x));
    }

    void Rec(int pos) {
        rec[++ top] = pos;
    }
    int Newpos() {
        if(top)
            return rec[top --];
        io.seekg(0, std :: ios :: end);
        int pos = io.tellp();
        return pos;
    }

    void ShiftBy(Node &node, int pos, int offset) {//pos=node.size
//        cout<<"node.size="<<node.size<<' '<<"pos="<<pos<<' '<<"pos+offset="<<pos + offset<<endl;
        node.val[pos] = node.val[pos + offset];
//        cout<<"????????????????????????"<<endl;
        node.son[pos] = node.son[pos + offset];
//        cout<<"why?"<<endl;
    }

    void Ins(TYPE key, Node &node) {
        int pos = node.size;
        for(int i = 0; i < node.size; ++ i) {
            if(key.first < node.val[i].first) {
                pos = i;
                break;
            }
        }
        for(int i = node.size; i > pos; -- i){
//            cout<<node.size+1-i<<endl;
            ShiftBy(node, i, -1);//i=node.size wrong!
        }
//        cout<<"a??"<<endl;
        node.val[pos] = key;
        ++ node.size;
    }

    void Del(TYPE key, Node &node) {
        int pos = -1;
        for(int i = 0; i < node.size; ++ i)
            if(node.val[i].first == key.first) {
                pos = i;
                break;
            }
        if(pos != -1) {
            -- node.size;
            for(int i = pos; i < node.size; ++ i)
                ShiftBy(node, i, 1);
        }
    }

    bool Exist_Leaf(TYPE key, int x) {
        Node node;
        Read(node, x);

        for(int i = 0; i < node.size; ++ i)
            if(node.val[i].first == key.first)
                return true;
        return false;
    }
    bool Exist_Leaf(TYPE_KEY key, int x) {
        Node node;
        Read(node, x);

        for(int i = 0; i < node.size; ++ i)
            if(node.val[i].first == key)
                return true;
        return false;
    }

    TYPE_VAL Find_Leaf(TYPE_KEY key, int x) {
        Node node;
        Read(node, x);

        int pos = node.size;
        for(int i = 0; i < node.size; ++ i)
            if(node.val[i].first == key)
                return node.val[i].second;

        return node.val[0].second;//没用，不会find不到的
    }
    void Find_Leaf(TYPE_KEY low, TYPE_KEY high, std :: vector<TYPE_VAL> &vec, int x) {
        Node node;
        Read(node, x);

        int pos = node.size;
        for(int i = 0; i < node.size; ++ i)
            if(low <= node.val[i].first) {
                pos = i;
                break;
            }
        if(pos == node.size) {
            x = node.right;
            if(x != -1) {
                pos = 0;
                Read(node, x);
            }
        }
        while(x != -1 && node.val[pos].first <= high) {
            vec.push_back(node.val[pos].second);
            ++ pos;
            if(pos == node.size) {
                x = node.right;
                if(x != -1) {
                    pos = 0;
                    Read(node, x);
                }
                else
                    break;
            }
        }
    }

    bool Change_Leaf(TYPE_KEY key, TYPE_VAL val, int x) {
        Node node;
        Read(node, x);

        int pos = node.size;
        bool suc = false;
        for(int i = 0; i < node.size; ++ i)
            if(node.val[i].first == key) {
                node.val[i].second = val;
                suc = true;
            }

        if(suc)
            Write(node, x);

        return suc;
    }

    void Recycle(int x = 0) {
        Node node;
        Read(node, x);

        if(!node.leaf)
            for(int i = 0; i < node.size; ++ i)
                Recycle(node.son[i]);

        Node init;
        Write(init, x);
        if(x)
            Rec(x);
    }

public:
    BPlusTree(const char *_file = "data.dat") {
//        cout << "bpt_str = " << _file << endl;
        strcpy(file, _file);
        std :: fstream in (file, std :: ios :: in | std :: ios :: binary);

        if(!in.is_open()) {
            std :: fstream out (file, std :: ios :: out | std :: ios :: binary);
            Node init;
            out.write(reinterpret_cast<char *>(&init), sizeof(init));
            out.close();
        }
        else
            in.close();
        io.open(file, std :: ios :: in | std :: ios :: out | std :: ios ::binary);
    }
    ~BPlusTree() {
        io.close();
    }

    void Clean() {
        Recycle();
    }
    bool Empty(int x = 0) {
        Node node;
        Read(node, x);
        return node.size == 0;
    }

    void Insert(TYPE key, int x = 0, int fa = -1, int fpos = -1) {
//        cerr<<"------Let's see the size of x!!!------"<<endl;
//        cout<<"x="<<x<<endl;
        Node node;
        Read(node, x);
//        node.Print();
        if(node.leaf){
//            cout<<"if"<<endl;
            Ins(key, node);}
        else {
//            cout<<"else"<<endl;
            int pos = node.size;
            for(int i = 0; i < node.size; ++ i)
                if(key.first < node.val[i].first) {
                    pos = i - 1;
                    break;
                }
            if(pos == -1)
                ++ pos;
            if(pos == node.size)
                -- pos;
//            puts("Insert2");
            Insert(key, node.son[pos], x, pos);
            Read(node, x);
        }
        if(fa != -1) {
            Node par;
            Read(par, fa);

            if(key.first < par.val[fpos].first)
                par.val[fpos] = key;
            if(node.size > Block) {

                Node newnode;
                newnode.size = Lim;
                int new_pos = Newpos();

                for(int i = Lim + 1; i < node.size; ++ i) {
                    newnode.val[i - Lim - 1] = node.val[i];
                    if(node.son[i] != -1) {
                        newnode.son[i - Lim - 1] = node.son[i];
                        node.son[i] = -1;
                        newnode.leaf = false;
                    }
                }
                node.size -= Lim;

                if(node.right != -1)
                    Write(new_pos, node.right + sizeof(int));
                newnode.right = node.right;
                newnode.left = x;
                node.right = new_pos;

                Write(newnode, new_pos);

                for(int i = par.size; i > fpos + 1; -- i)
                    ShiftBy(par, i, -1);
                par.val[fpos + 1] = newnode.val[0];
                par.son[fpos + 1] = new_pos;
                ++ par.size;
            }
            Write(par, fa);
        }
        else {
            assert(x == 0);

            if(node.size > Block) {

                Node newnode;
                newnode.size = Lim;
                int new_pos = Newpos();

                for(int i = Lim + 1; i < node.size; ++ i) {
                    newnode.val[i - Lim - 1] = node.val[i];
                    if(node.son[i] != -1) {
                        newnode.son[i - Lim - 1] = node.son[i];
                        node.son[i] = -1;
                        newnode.leaf = false;
                    }
                }
                node.size -= Lim;

                if(node.right != -1)
                    Write(new_pos, node.right + sizeof(int));
                newnode.right = node.right;
                newnode.left = x;
                node.right = new_pos;

                Write(newnode, new_pos);

                int new_xpos = Newpos();
                x = new_xpos;
                if(node.right != -1)
                    Write(x, node.right + sizeof(int));
                if(node.left != -1)
                    Write(x, node.left);

                Node newroot;
                newroot.size = 2;
                newroot.val[0] = node.val[0];
                newroot.son[0] = new_xpos;
                newroot.val[1] = newnode.val[0];
                newroot.son[1] = new_pos;
                newroot.leaf = false;

                Write(newroot, 0);
            }
        }
//        cout<<"wow"<<endl;
        Write(node, x);
    }

    void Delete(TYPE key, int x = 0, int fa = -1, int fpos = -1) {
        Node node;
        Read(node, x);

        if(node.leaf)
            Del(key, node);
        else {
            int pos = node.size;
            for(int i = 0; i < node.size; ++ i)
                if(key.first < node.val[i].first) {
                    pos = i - 1;
                    break;
                }
            if(pos == node.size)
                -- pos;
            if(pos == -1)
                return;
            Delete(key, node.son[pos], x, pos);
            Read(node, x);
        }

        if(fa != -1) {
            Node par;
            Read(par, fa);

            if(par.val[fpos].first == key.first)
                par.val[fpos] = node.val[0];
            if(node.size < Lim) {
                if(fpos != -1 && fpos > 0) {
                    int left_pos = par.son[fpos - 1];
                    Node left;
                    Read(left, left_pos);

                    if(left.size > Lim) {
                        for(int i = node.size; i; -- i)
                            ShiftBy(node, i, -1);
                        ++ node.size;
                        -- left.size;
                        node.val[0] = left.val[left.size];
                        if(left.son[left.size] != -1) {
                            node.son[0] = left.son[left.size];
                            left.son[left.size] = -1;
                        }

                        par.val[fpos] = node.val[0];

                        Write(node, x);
                    }
                    else {
                        for(int i = 0; i < node.size; ++ i) {
                            left.val[left.size + i] = node.val[i];
                            if(node.son[i] != -1) {
                                left.son[left.size + i] = node.son[i];
                                node.son[i] = -1;
                            }
                        }
                        left.size += node.size;
                        if(node.right != -1) {
                            left.right = node.right;
                            Write(left_pos, node.right + sizeof(int));
                            node.right = -1;
                        }
                        else
                            left.right = -1;

                        -- par.size;
                        for(int i = fpos; i < par.size; ++ i)
                            ShiftBy(par, i, 1);

                        Rec(x);
                    }
                    Write(left, left_pos);
                }
                else if(fpos != -1 && fpos < par.size - 1) {
                    int right_pos = par.son[fpos + 1];
                    Node right;
                    Read(right, right_pos);

                    if(right.size > Lim) {
                        node.val[node.size] = right.val[0];
                        if(right.son[0] != -1) {
                            node.son[node.size] = right.son[0];
                            right.son[0] = -1;
                        }
                        ++ node.size;
                        -- right.size;
                        for(int i = 0; i < right.size; ++ i)
                            ShiftBy(right, i, 1);

                        par.val[fpos + 1] = right.val[0];

                        Write(right, right_pos);
                    }
                    else {
                        for(int i = 0; i < right.size; ++ i) {
                            node.val[node.size + i] = right.val[i];
                            if(right.son[i] != -1) {
                                node.son[node.size + i] = right.son[i];
                                right.son[i] = -1;
                            }
                        }
                        node.size += right.size;

                        if(right.right != -1) {
                            node.right = right.right;
                            Write(x, right.right + sizeof(int));
                            right.right = -1;
                        }
                        else
                            node.right = -1;

                        -- par.size;
                        for(int i = fpos + 1; i < par.size; ++ i)
                            ShiftBy(par, i, 1);

                        Rec(right_pos);
                    }

                    Write(node, x);
                }
            }
            else
                Write(node, x);
            Write(par, fa);
        }
        else {
            if(!node.leaf && node.size == 1) {
                int root_pos = node.son[0];
                Node root;
                Read(root, root_pos);
                root_pos = 0;
                Write(root, root_pos);
                if(root.right != -1)
                    Write(root_pos, root.right + sizeof(int));
                if(root.left != -1)
                    Write(root_pos, root.left);
                Rec(x);
            }
            else
                Write(node, x);
        }
    }

    bool Exist(TYPE key, int x = 0) {
        Node node;
        Read(node, x);

        if(node.leaf)
            return Exist_Leaf(key, x);
        else {
            int pos = node.size;
            for(int i = 0; i < node.size; ++ i)
                if(key.first < node.val[i].first) {
                    pos = i - 1;
                    break;
                }
            if(pos == -1)
                return false;
            if(pos == node.size)
                -- pos;
            return Exist(key, node.son[pos]);
        }
    }
    bool Exist(TYPE_KEY key, int x = 0) {
        Node node;
        Read(node, x);

        if(node.leaf)
            return Exist_Leaf(key, x);
        else {
            int pos = node.size;
            for(int i = 0; i < node.size; ++ i)
                if(key < node.val[i].first) {
                    pos = i - 1;
                    break;
                }
            if(pos == -1)
                return false;
            if(pos == node.size)
                -- pos;
            return Exist(key, node.son[pos]);
        }
    }

    TYPE_VAL Find(TYPE_KEY key, int x = 0) {
        Node node;
        Read(node, x);

        if(node.leaf)
            return Find_Leaf(key, x);
        else {
            int pos = node.size;
            for(int i = 0; i < node.size; ++ i)
                if(key < node.val[i].first) {
                    pos = i - 1;
                    break;
                }
            if(pos == node.size)
                -- pos;
            if(pos == -1)
                ++ pos;
            return Find(key, node.son[pos]);
        }
    }
    void Find_(TYPE_KEY low, TYPE_KEY high, std :: vector<TYPE_VAL> &vec, int x = 0) {
        Node node;
        Read(node, x);

        if(high < low)
            return;
        if(node.leaf)
            Find_Leaf(low, high, vec, x);
        else {
            int pos = node.size;
            for(int i = 0; i < node.size; ++ i)
                if(low < node.val[i].first) {
                    pos = i - 1;
                    break;
                }
            if(pos == node.size)
                -- pos;
            if(pos == -1)
                ++ pos;
            Find_(low, high, vec, node.son[pos]);
        }
    }

    bool Change(TYPE_KEY key, TYPE_VAL val, int x = 0) {
        Node node;
        Read(node, x);

        if(node.leaf)
            return Change_Leaf(key, val, x);
        else {
            int pos = node.size;
            for(int i = 0; i < node.size; ++ i)
                if(key < node.val[i].first) {
                    pos = i - 1;
                    break;
                }
            if(pos == node.size)
                -- pos;
            if(pos == -1)
                return false;
            bool tmp = Change(key, val, node.son[pos]);
            if(node.val[pos].first == key)
                node.val[pos].second = val;

            Write(node, x);
            return tmp;
        }
    }

    // ===debug=== //
    void Dfs(int x) {
        Node tmp;
        Read(tmp, x);
        std :: cout << "Pos = " << x << ": " << std :: endl;
        tmp.Print();
        for(int i = 0; i < tmp.size; ++ i)
            if(tmp.son[i] != -1)
                Dfs(tmp.son[i]);
    }
    void Test() {
        std :: cout << "Get Test Start ===============================================================" << std :: endl;
        Dfs(0);
        std :: cout << "Get Test End ===============================================================" << std :: endl;
    }
};

#endif
