nodemon 은 global 설치되어 있어야 함.
확인하려면,
$ npm list -g
확인 후, package.json 이 포함된 현재 프로젝트 디렉토리에서 다음을 실행
$ npm i
$ nodemon ./server.js