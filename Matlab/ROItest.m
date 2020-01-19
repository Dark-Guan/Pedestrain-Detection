%%ROI Testing
close all;
clear all;
clc;
I=imread('Test.png');
I=rgb2gray(I);
I=I(1:100,1:200);
[m,n]=size(I);
I_BW=I;
%��ֵ��ͼ��
for Row1=1:m
    for Clo1=1:n
        if I(Row1,Clo1)>190
            I_BW(Row1,Clo1)=255;
        else
            I_BW(Row1,Clo1)=0;
        end
    end
end
imshow(I)
%figure,imhist(I)
figure,imshow(I_BW)
I_BW_morph=bwmorph(I_BW,'erod',0.8);%��̬ѧ��ʴԭ��
figure,imshow(I_BW_morph)
Filter=[-1,-1,-1;-1,8,-1;-1,-1,-1];%��ͨ�˲������ڳ���ֵͼ��ı߽�����
I_Filter=imfilter(I_BW_morph,Filter);%ʹ���˲��ķ����õ���ͼ��ı߽粿���ǰ�ɫ�ģ��߽��б仯���Ծ�����ͨ�˲�����Ϊ��ɫ
figure,imshow(I_Filter)
I_Edge=I_Filter;
I_Edge(:,2)=0;%���ڲ��õ����˲����ķ�����ʵ�ֱ�Ե����ȡ����ͼ��ı߽粿�ֻ������ɫ�ı仯�����������Ҫ��ͼ���Ե�Ĳ�����Ϊ0
I_Edge(:,n-1)=0;
I_Edge(2,:)=0;
I_Edge(m-1,:)=0;
figure,imshow(I_Edge)
I_Edge=uint8(I_Edge);%ͼ��uint8��
ROI_Buffer=uint8(zeros(m,n,5));%�ĸ�ģ�建������
C_Shape=zeros(4,1);
Label=0;

for Row2=1:m
    for Clo2=1:n
        if I_Edge(Row2,Clo2)==1
            Label=Label+1
            [ROI_Buffer(:,:,Label),C_Shape(Label,1)]=Pixel_Search(I_Edge,Row2,Clo2,m,n);%�ѵ�ǰ�����ݸ���ģ��
            figure,imshow(255*ROI_Buffer(:,:,Label));%��ʾ���õ��Ķ�Ӧ���ΪLabel��ͼ��߽磬������Ҫע��������ǣ���ͼ���ɫ�ʵȼ���ͬ����˵õ���ͼ�����ȫ�Ǻ�ɫ����Ҫ����255��uint8��
            C_Shape(Label,1)
            I_Edge=I_Edge-ROI_Buffer(:,:,Label);%ȥ��ԭͼ���Ѿ���ȡ��ɵĲ���
        end
    end
end