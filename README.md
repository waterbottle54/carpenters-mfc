# 카펜터 (Carpenter) : 3D 모델 설계 소프트웨어
<p align="right">Copyright © 2016 조성원(Sung Won Jo)</p>

 ## Introduction
 
 3D 모델 설계 (3D Model Design) | 매개변수 곡면(Parametric Surfaces)
:-----------------------------:|:------------------------:
![](https://github.com/waterbottle54/carpenters-mfc/blob/main/screenshots/car.png) | ![](https://github.com/waterbottle54/carpenters-mfc/blob/main/screenshots/dialog.png)

 아핀 변환 (Affine Transformation) | 데이터 보존(Serialization)
:-----------------------------:|:------------------------:
![](https://github.com/waterbottle54/carpenters-mfc/blob/main/screenshots/transformation.png) | ![](https://github.com/waterbottle54/carpenters-mfc/blob/main/screenshots/serialization.png)

 응용 - 레이싱 게임 제작에 위 모델 사용 (Applied case) |
:-----------------------------:|
![](https://github.com/waterbottle54/carpenters-mfc/blob/main/screenshots/dashboard.png)

 * **Carpenter**는 **MFC / C++** 로 작성된 **Desktop** 3D 모델 설계 소프트웨어입니다.<br>.

 ## Getting Started
> ### Dependencies
> * Windows: **10, 11**
> * Visual Studio Redistributable

 ## Funtionality
> ### Modeling
> * 매개변수 곡면 벡터함수 S(u, v)를 이용해 다양한 곡면을 생성한다.
> * 3차원 아핀 변환을 적용하여 응용한다. (ex. 원 -> 타원)
> ### Etc.
> * 프로젝트 파일 및 모델 파일을 저장, 불러오기(Serialization) 할 수 있다.

 ## Project Overview
> ### Language
> C++

> ### IDE
> Visual Studio 2015

> ### Framework
> MFC
 
> ### Architecture
> * Document-View Architecture
 
> ### 3D Graphics
> * 3D 렌더링에 OpenGL을 사용하였다.
> * Normal vector lighting, Unprojection 등의 기법이 사용되었다.

 ## Author
 * 조성원 (Sung Won Jo)
 
     📧 waterbottle54@naver.com
   
     📚 [Portfolio](https://www.devsungwonjo.pe.kr/)
   
     📹 [YouTube Channel](https://github.com/waterbottle54)
   
