< 오류 발생 >
utils/winston.js에서 filename 중 %DATE% 부분을 %DAT%라고 오타로
적어서, 파일형식이 log도 아니였고, 파일명도 이상하게 나와 고쳤습니다.
이 오류를 꽤나 늦게 발견해서 server.js의 get 부분에서 파일을 읽어오는
readFile 함수가 이상한 것을 읽어오게 되어서, retData에 이상한 값이
들어가는 걸 파악하고 오류를 발견한 지점부터 차근차근 올라가다 결국
오류를 발견하여 디버깅하게 되었습니다.



< 개발하며 새로 알게된 부분 >
log 파일을 생성하여 해당 log에 찍힌 데이터를 웹사이트에 전송하는
프로젝트를 진행하였습니다.
winston에 대한 라이브러리에 대해 알게되었고, 어떤 상황에서 써야될지 파악했습니다.
indexOf와 slice라는 기본 함수를 다시 써보게 됨으로써 잊어버렸던 기억들을 다시 상기시켰고, 이를 통해 심화적으로 insert라는 함수를 자체 제작할 수 있었습니다.
const로 변수를 선언해줄 때 const {a}와 const a와의 차이를 명확히 알았습니다.
