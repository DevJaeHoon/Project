const express = require("express");
const morgan = require("morgan");

// node에서 제공하는 파일 읽는 라이브러리
const fs = require("fs");

const {logger} = require("./utils/winston");

const app = express();
const port = process.env.PORT || 8080;
app.set("port", port);
app.use(morgan("dev"));
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// ejs를 사용하기 위한 default
app.set("view engine", "ejs");
// views 파일의 index.ejs로 접근
app.use(express.static(__dirname + "/views"));

// 밑에 써줄 insert 함수 생성
const insert = (str,index,target) => {
  const front = str.slice(0, index);
  const back = str.slice(index,str.length);
  return front + target + back;
};



app.get("/", (req, res) => {
  logger.error("error 메시지");
  logger.warn("warn 메시지");
  logger.info("info 메시지");
  logger.http("http 메시지");
  logger.debug("debug 메시지");
  fs.readFile("./logs/2022-02--18--14.log", "utf8",(err,data)=>{
    // data는 string type로 넘어옴
    let retData = data;
    let idx = -1;
    // 정확한 log의 갯수가 파악되지 않기 때문에 while이 적합함
    while(1) {
      // indexOf(a,b) : a라는 것을 b에서 부터 찾기, 괄호의 끝을 찾는다.
      idx = retData.indexOf("}", idx + 1);
      // indexOf함수는 "}"을 찾게 되면 -1 출력
      if(idx === -1){
        break;
      }
      // } 뒤에 ,을 붙여 JSON 타입으로 바꿔주기 위해
      retData = insert(retData,idx+1,",");
    }
    // console.log(retData);  // console.log를 찍어보니 enter가 두번 찍혀있었음
    // 배열안에 넣어 JSON 타입으로 변경
    // JSON => [{"":""},{"":""}]
    retData = "[" + retData.slice(0, retData.length - 3) + "]";
    // 객체를 JSON화
    retData = JSON.parse(retData);
    // console.log(retData);
    // ejs에서 sendData라는 변수로 받을 것인기 때문에
    return res.render("index", {sendData:retData});
  });
  // res.send("Hello world");
  // template engine(ejs)을 사용할 때 render 사용
  // return을 하지 않으면
  
});

app.listen(app.get("port"), () => {
  console.log(`this server listening on ${app.get("port")}`);
});
