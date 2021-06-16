# final

### Setup<br>
- xbee : TX<->D10, RX<->D9<br>
- openMV : P4<->D0, P5<->D1<br>
- Ping : D12<br>
- encoder : D11<br>
- continuous servo : D5、D6<br>

### My design<br>
1. Follow the line first.<br>
2. Navigation by apriltag.<br>
3. Follow the line again.<br>
4. Navigation by april tag and go back.<br>
5. Navigation by april tag again.<br>
6. Use Ping to detect the distance for parking.<br> 
7. Parking.<br>
8. Let openMV to scan the picture and predict label.<br>

### Application<br>
- xbee for transport some information<br>
- line following<br>
- april tag for location identification<br>
- ping for detect distance<br>
- parking<br>
- image classification for numerical number<br>
- openMV processing<br>

### Some Result<br>
- Line following and april tag location identification
![S__41484305](https://user-images.githubusercontent.com/79573591/122201845-0a391f80-cecf-11eb-90fc-55efd514248e.jpg)
- Line following and april tag location identification
![S__41484307](https://user-images.githubusercontent.com/79573591/122201866-0e653d00-cecf-11eb-984d-752cc15f5148.jpg)
- Start go back
![S__41484308](https://user-images.githubusercontent.com/79573591/122201880-11f8c400-cecf-11eb-9d04-31900387ab13.jpg)
- April tag location identifiaction
![S__41484309](https://user-images.githubusercontent.com/79573591/122201883-13c28780-cecf-11eb-812c-a08047aa7d18.jpg)
- Ping distance detection
![S__41484310](https://user-images.githubusercontent.com/79573591/122201899-17eea500-cecf-11eb-8c1d-dd8ba3d4fd59.jpg)
- Parking
![S__41484311](https://user-images.githubusercontent.com/79573591/122201909-1ae99580-cecf-11eb-89ff-465a84f58947.jpg)
- image classification
![S__41484312](https://user-images.githubusercontent.com/79573591/122201933-1f15b300-cecf-11eb-9ce1-013cb23c3085.jpg)
- get information of xbee
![S__41484313](https://user-images.githubusercontent.com/79573591/122201942-20df7680-cecf-11eb-90d0-ab50b834510b.jpg)
