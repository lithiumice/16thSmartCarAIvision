#include "vector"
using namespace std;

//int direction[8][2] = {{1,  0},
//                       {1,  -1},
//                       {0,  -1},
//                       {-1, -1},
//                       {-1, 0},
//                       {-1, 1},
//                       {0,  1},
//                       {1,  1}};
//vector <FM> FM(contours[0].size());
//vector<int> d(contours[0].size());
//vector<int> k(contours[0].size());
//int CornersN = 300;
//vector <Point2f> vecCorners(CornersN);
//int startDirect = 0;
//int FindStartPoint = 0;
//int FindPoint = 0;
//
//
//void search() {
//    //开始跟踪
//    int X = startX;
//    int Y = startY;
//    int NextPtValue;
//    int nextX, nextY;
//    int i = 0;
//
//    while (!FindStartPoint) {
//        FindPoint = 0;
//        while (!FindPoint) {
//            nextX = X + direction[startDirect][1];
//            nextY = Y + direction[startDirect][0];
//            NextPtValue = drawing.at<uchar>(nextX, nextY);
//
//            if (NextPtValue != 0 && nextX >= 0 && nextY >= 0) {
//                FM[i].
//                        ai = startDirect;
//                FM[i].
//                        x = nextY;
//                FM[i].
//                        y = nextX;
//                i++;
//                FindPoint = 1;
//                X = nextX;
//                Y = nextY;
//                if (X ==
//                    startX && Y
//                              == startY) {
//                    FindStartPoint = 1;
//                }
//                startDirect = startDirect - 2;
//                if (startDirect < 0)
//                    startDirect = startDirect + 8;
//            } else {
//                startDirect = startDirect + 1;
//                if (startDirect >= 8)
//                    startDirect = startDirect - 8;
//            }
//        }
//    }
//
//
//}

BOOL PickBoundary(BYTE* nVerct,int (*Coordinate)[2])
{
    if(NULL == m_lpBits)
        return FALSE;

    struct EdgePoint
    {
        BYTE nCurrenVerct;       //当前矢量，即在轮廓跟踪中的前一个搜索方向
        CPoint CurrenPoint;      //当前点的坐标
    };

//CArray <EdgePoint,EdgePoint&> TraceArray;

//获取位图的宽，高，以及每行字节数
    int nWidth=m_nWidth;
    int nHeight=m_nHeight;
    int nByteWidth=BYTE_PER_LINE(m_nWidth,m_nBitCount);


//循环变量
    int i,j;

//开辟一块新的空间并初始化
    BYTE *pData=new BYTE[nByteWidth*nHeight];
    memset(pData,255,nByteWidth*nHeight);

//像素值
    unsigned char pixel;

//是否找到起始点及回到起始点
    bool bFindStartPoint;

//是否扫描到一个边界点
    bool bFindPoint;

    typedef struct
    {
        int Width;
        int Height;
    }Position;

//起始边界点与当前边界点
    Position StartPoint,CurrentPoint;

//八个方向和起始扫描方向，依次是左上方、上方、右上方、右方、右下方、下方、左下方和左方。
    int Direction[8][2]={ {-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0} };
    int BeginDirect;


//先找到最左下方的边界点
    bFindStartPoint = false;
    for (j = 1;j < nHeight-1 && !bFindStartPoint;j++)
    {
        for(i = 1;i < nWidth-1 && !bFindStartPoint;i++)
        {
            //取得当前指针处的像素值
            pixel =*(m_lpBits+nWidth*j+i);

            if(pixel == 0)
            {
                bFindStartPoint = true;
                StartPoint.Height = j;
                StartPoint.Width = i;

                // 指向目标图像倒数第j行，第i个象素的指针
                *(pData+nWidth*j+i) = 0;
            }
        }
    }

//由于起始点是在左下方，故起始扫描沿左上方向
    BeginDirect = 0;
//跟踪边界
    bFindStartPoint = false;

    CurrentPoint.Height = StartPoint.Height;
    CurrentPoint.Width = StartPoint.Width;

    int index=0;
    memset(nVerct,0,9999);


    while(!bFindStartPoint)
    {
        //从起始点一直找边界，直到再次找到起始点为止
        bFindPoint = false;
        while(!bFindPoint)
        {
            //沿扫描方向，这里是左上方向获取一个像素
            pixel = *(m_lpBits+nWidth*(CurrentPoint.Height + Direction[BeginDirect][1])+
                      CurrentPoint.Width + Direction[BeginDirect][0]);
            if(pixel == 0)
            {
                bFindPoint = true;

                //存储边界点的矢量方向
                nVerct[index] = (BYTE)BeginDirect;
                //存储边界点的坐标
                Coordinate[index][0] = CurrentPoint.Width;
                Coordinate[index][1] = CurrentPoint.Height;
                index++;


                CurrentPoint.Width = CurrentPoint.Width + Direction[BeginDirect][0];
                CurrentPoint.Height = CurrentPoint.Height + Direction[BeginDirect][1];

                //如果一圈以后找到了起始点，那么边界寻找完成
                if(CurrentPoint.Height == StartPoint.Height &&
                   CurrentPoint.Width == StartPoint.Width)
                {
                    bFindStartPoint = true;
                }
                //将轮廓在新的位图中标示出来
                *(pData+nWidth*CurrentPoint.Height+CurrentPoint.Width) = 0;
                //扫描的方向逆时针旋转两格
                //为什么？因为逆时针旋转三格的象素绝对不是边界，不然当前点就不会是边界。
                BeginDirect--;
                if(BeginDirect == -1)
                    BeginDirect = 7;
                BeginDirect--;
                if(BeginDirect == -1)
                    BeginDirect = 7;
            }
            else
            {
                //扫描方向顺时针旋转一格
                BeginDirect++;
                if(BeginDirect == 8)
                    BeginDirect = 0;
            }

        }
    }