#include"vrmp.h"
#include <climits>

using namespace std;
Replace::Replace (){
int i;
// 设定总得访问页数，并分配相应的引用页号和淘汰页号记录数组空间
cout <<"Please input page numbers:";
cin>> PageNumber;
ReferencePage = new int [sizeof (int)* PageNumber];
EliminatePage = new int [sizeof (int)* PageNumber];
cout << "Please input reference page string";
// 输入引用页号序列 (页面走向), 初始化引用页数组
for (i=0;i< PageNumber; i++)
cin>>ReferencePage [i]; //引用页暂存引用数组/
// 设定内存实页数 (帧数), 并分配相应的实页号记录数组空间 (页号栈)
cout <<"Please input page frames :";
cin>> FrameNumber;
PageFrames = new int [sizeof (int) * FrameNumber];
}
Replace::~Replace (){}
void Replace::InitSpace (char * MethodName) {
int i;
cout <<endl << MethodName <<endl;
FaultNumber=0;
// 引用还未开始，-1 表示无引用页
for (i=0;i< PageNumber; i++)
EliminatePage[i]=-1;
for (i= 0; i< FrameNumber; i++)
PageFrames [i]=-1;
}
//分析统计选择的算法对于当前输入的页面走向的性能/
void Replace::Report (void){
// 报告淘汰页顺序
cout <<endl <<"Eliminate page:";
for (int i=0; EliminatePage [i]!=-1;i++) cout<<EliminatePage [i]<<";";
// 报告缺页数和缺页率
cout <<endl <<"Number of page faults ="<< FaultNumber <<endl;
cout<<setw (6)<<setprecision (3);
cout<<"Rate of page faults ="<<100*(float) FaultNumber/(float) PageNumber <<"%"<<endl;
}
// 最近最旧未用置换算法
void Replace::Lru (void) {
int i,j,k,l,next;
InitSpace ("LRU");
// 循环装入引用页
for (k=0,l=0;k< PageNumber; k++){
next=ReferencePage [k];
// 检测引用页当前是否已在实存
for (i=0; i<FrameNumber;i++){
if (next== PageFrames [i]){
// 引用页已在实存将其调整到页记录栈顶
next= PageFrames [i];
break;
}
}
if (PageFrames [0]==next){
// 如果引用页已放栈顶，则为不缺页，报告当前内存页号
for (j=0;j<FrameNumber;j++)
if (PageFrames [j]>=0)
cout << PageFrames [j]<<"";
cout<<endl;
continue;// 继续装入下一页
}
else {
// 如果引用页还未放栈顶，则为缺页，缺页数加 1
// 栈底页号记入淘汰页数组中
EliminatePage [l]= PageFrames [FrameNumber-1];
FaultNumber++;
// 向下压栈
for (j=FrameNumber-1;j>0;j--)
PageFrames [j]= PageFrames [j-1];
PageFrames [0]=next;//引用页放栈顶/
// 报告当前实存中页号
for (j=0;j<FrameNumber;j++)
if (PageFrames [j]>=0)
cout<< PageFrames [j]<<" ";
// 报告当前淘汰的页号
if (EliminatePage [l]>=0)
cout<<"->"<<EliminatePage [l++]<<endl;
else
cout<<endl;
}
}
// 分析统计选择的算法对于当前引用的页面走向的性能
Report ();
}
// 先进先出置换算法
void Replace::Fifo (void){
int i,j,k,l,next;
InitSpace ("FIFO");
// 循环装入引用页
for (k=0,j=0,l=0;k< PageNumber; k++){
next=ReferencePage [k];
for (i=0;i<FrameNumber;i++)
if (next==PageFrames [i])
break;
// 如果引用页已在实存中，报告实存页号
if (i<FrameNumber){
for (i=0;i<FrameNumber;i++)
if (PageFrames [i]>=-0)
cout<<PageFrames [i]<<" ";
cout<<endl;
continue;// 继续引用下一页
}
// 引用页不在实存中，缺页数加 1
FaultNumber++;
j=(j+1)% FrameNumber;//最先入页号循环下移/
for(i=0; i<FrameNumber; i++)
EliminatePage [l]= PageFrames [j];//最先入页号记入淘汰页数组/
PageFrames [j]=next;//引用页号放最先入页号处/
// 报告当前实存页号和淘汰页号
for (i=0;i<FrameNumber;i++)
if (PageFrames [i]>=-0)
cout<<PageFrames [i]<<" ";
if (EliminatePage [l]>=0)
cout<<"->"<<EliminatePage [l++]<<endl;
else
cout<<endl;
}
/*分析统计选择的算法对于当前引用的页面走向的性能*/
Report ();
}
// 未实现的其他页置换算法入口
//
void Replace::Clock(void) {
    InitSpace("CLOCK");
    // 初始化时钟指针和引用位
    clockHand = 0;
    referenceBit = new int[FrameNumber];
    for (int i = 0; i < FrameNumber; i++) {
        referenceBit[i] = 0;
    }

    for (int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;
        
        // 检查页面是否在内存中
        for (int i = 0; i < FrameNumber; i++) {
            if (PageFrames[i] == next) {
                referenceBit[i] = 1;  // 设置引用位
                found = true;
                break;
            }
        }
        
        if (found) {
            // 输出当前内存状态
            for (int j = 0; j < FrameNumber; j++) {
                if (PageFrames[j] >= 0) cout << PageFrames[j] << " ";
            }
            cout << endl;
            continue;
        }
        
        // 缺页处理
        FaultNumber++;
        int victim = -1;
        
        // 查找淘汰页面
        while (victim == -1) {
            if (referenceBit[clockHand] == 0) {
                victim = clockHand;
            } else {
                referenceBit[clockHand] = 0;  // 清除引用位
            }
            clockHand = (clockHand + 1) % FrameNumber;
        }
        
        // 记录淘汰页
        EliminatePage[FaultNumber-1] = PageFrames[victim];
        
        // 装入新页面
        PageFrames[victim] = next;
        referenceBit[victim] = 1;  // 设置引用位
        
        // 输出当前状态
        for (int j = 0; j < FrameNumber; j++) {
            if (PageFrames[j] >= 0) cout << PageFrames[j] << " ";
        }
        cout << "-> " << EliminatePage[FaultNumber-1] << endl;
    }
    Report();
    delete[] referenceBit;
}
//
//
void Replace::Eclock(void) {
    InitSpace("ECLOCK");
    // 初始化时钟指针、引用位和修改位
    clockHand = 0;
    referenceBit = new int[FrameNumber];
    modifyBit = new int[FrameNumber];
    for (int i = 0; i < FrameNumber; i++) {
        referenceBit[i] = 0;
        modifyBit[i] = rand() % 2; // 随机初始化修改位
    }

    for (int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;
        
        // 检查页面是否在内存中
        for (int i = 0; i < FrameNumber; i++) {
            if (PageFrames[i] == next) {
                referenceBit[i] = 1;  // 设置引用位
                found = true;
                break;
            }
        }
        
        if (found) {
            // 输出当前内存状态
            for (int j = 0; j < FrameNumber; j++) {
                if (PageFrames[j] >= 0) cout << PageFrames[j] << " ";
            }
            cout << endl;
            continue;
        }
        
        // 缺页处理
        FaultNumber++;
        int victim = -1;
        int rounds = 0;
        
        // 查找淘汰页面（四类优先级）
        while (victim == -1 && rounds < 4) {
            for (int i = 0; i < FrameNumber; i++) {
                int idx = (clockHand + i) % FrameNumber;
                if (referenceBit[idx] == 0 && modifyBit[idx] == 0 && victim == -1) {
                    victim = idx;  // 第一类: (0,0)
                }
            }
            
            if (victim == -1) {
                for (int i = 0; i < FrameNumber; i++) {
                    int idx = (clockHand + i) % FrameNumber;
                    if (referenceBit[idx] == 0 && modifyBit[idx] == 1 && victim == -1) {
                        victim = idx;  // 第二类: (0,1)
                    } else if (referenceBit[idx] == 1) {
                        referenceBit[idx] = 0;  // 清除引用位
                    }
                }
            }
            clockHand = (clockHand + 1) % FrameNumber;
            rounds++;
        }
        
        // 如果前两轮没找到，随机选择
        if (victim == -1) {
            victim = rand() % FrameNumber;
        }
        
        // 记录淘汰页
        EliminatePage[FaultNumber-1] = PageFrames[victim];
        
        // 装入新页面
        PageFrames[victim] = next;
        referenceBit[victim] = 1;  // 设置引用位
        modifyBit[victim] = rand() % 2;  // 随机设置修改位
        
        // 输出当前状态
        for (int j = 0; j < FrameNumber; j++) {
            if (PageFrames[j] >= 0) cout << PageFrames[j] << " ";
        }
        cout << "-> " << EliminatePage[FaultNumber-1] << endl;
    }
    Report();
    delete[] referenceBit;
    delete[] modifyBit;
}



void Replace::Lfu(void) {
    InitSpace("LFU");
    // 初始化访问计数器和最后使用时间
    accessCount = new int[FrameNumber];
    lastUsed = new int[FrameNumber];
    for (int i = 0; i < FrameNumber; i++) {
        accessCount[i] = 0;
        lastUsed[i] = -1;  // -1 表示未使用
    }

    for (int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;
        int pos = -1;

        // 检查页面是否在内存中
        for (int i = 0; i < FrameNumber; i++) {
            if (PageFrames[i] == next) {
                accessCount[i]++;  // 增加访问计数
                lastUsed[i] = k;   // 更新最后访问时间
                found = true;
                pos = i;
                break;
            }
        }

        if (found) {
            // 输出当前内存状态
            for (int j = 0; j < FrameNumber; j++) {
                if (PageFrames[j] >= 0) cout << PageFrames[j] << " ";
            }
            cout << endl;
            continue;
        }

        // 缺页处理
        FaultNumber++;
        int victim = -1;
        int minCount = INT_MAX;
        int minTime = INT_MAX;

        // 查找访问次数最少的页面
        for (int i = 0; i < FrameNumber; i++) {
            if (PageFrames[i] == -1) {  // 优先使用空帧
                victim = i;
                break;
            }
            if (accessCount[i] < minCount ||
                (accessCount[i] == minCount && lastUsed[i] < minTime)) {
                minCount = accessCount[i];
                minTime = lastUsed[i];
                victim = i;
            }
        }

        // 记录淘汰页
        if (PageFrames[victim] != -1) {
            EliminatePage[FaultNumber-1] = PageFrames[victim];
        } else {
            EliminatePage[FaultNumber-1] = -1;  // 空帧不淘汰
        }

        // 装入新页面
        PageFrames[victim] = next;
        accessCount[victim] = 1;  // 初始化访问计数
        lastUsed[victim] = k;     // 设置最后访问时间

        // 输出当前状态
        for (int j = 0; j < FrameNumber; j++) {
            if (PageFrames[j] >= 0) cout << PageFrames[j] << " ";
        }
        if (EliminatePage[FaultNumber-1] >= 0) {
            cout << "-> " << EliminatePage[FaultNumber-1] << endl;
        } else {
            cout << endl;
        }
    }
    Report();
    delete[] accessCount;
    delete[] lastUsed;
}



void Replace::Mfu(void) {
    InitSpace("MFU");
    // 初始化访问计数器和最后使用时间
    accessCount = new int[FrameNumber];
    lastUsed = new int[FrameNumber];
    for (int i = 0; i < FrameNumber; i++) {
        accessCount[i] = 0;
        lastUsed[i] = -1;  // -1 表示未使用
    }

    for (int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;
        int pos = -1;

        // 检查页面是否在内存中
        for (int i = 0; i < FrameNumber; i++) {
            if (PageFrames[i] == next) {
                accessCount[i]++;  // 增加访问计数
                lastUsed[i] = k;   // 更新最后访问时间
                found = true;
                pos = i;
                break;
            }
        }

        if (found) {
            // 输出当前内存状态
            for (int j = 0; j < FrameNumber; j++) {
                if (PageFrames[j] >= 0) cout << PageFrames[j] << " ";
            }
            cout << endl;
            continue;
        }

        // 缺页处理
        FaultNumber++;
        int victim = -1;
        int maxCount = -1;
        int minTime = INT_MAX;

        // 查找访问次数最多的页面
        for (int i = 0; i < FrameNumber; i++) {
            if (PageFrames[i] == -1) {  // 优先使用空帧
                victim = i;
                break;
            }
            if (accessCount[i] > maxCount ||
                (accessCount[i] == maxCount && lastUsed[i] < minTime)) {
                maxCount = accessCount[i];
                minTime = lastUsed[i];
                victim = i;
            }
        }

        // 记录淘汰页
        if (PageFrames[victim] != -1) {
            EliminatePage[FaultNumber-1] = PageFrames[victim];
        } else {
            EliminatePage[FaultNumber-1] = -1;  // 空帧不淘汰
        }

        // 装入新页面
        PageFrames[victim] = next;
        accessCount[victim] = 1;  // 初始化访问计数
        lastUsed[victim] = k;     // 设置最后访问时间

        // 输出当前状态
        for (int j = 0; j < FrameNumber; j++) {
            if (PageFrames[j] >= 0) cout << PageFrames[j] << " ";
        }
        if (EliminatePage[FaultNumber-1] >= 0) {
            cout << "-> " << EliminatePage[FaultNumber-1] << endl;
        } else {
            cout << endl;
        }
    }
    Report();
    delete[] accessCount;
    delete[] lastUsed;
}


int main (int argc,char *argv []){
 Replace * vmpr = new Replace ();
vmpr->Lru ();
vmpr->Fifo ();
vmpr->Clock();
vmpr->Eclock();
vmpr->Lfu();
vmpr->Mfu();
return 0;
}
