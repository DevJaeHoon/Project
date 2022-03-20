const express = require("express");
const morgan = require("morgan");

const cors = require("cors");
const axios = require("axios");
const dotenv = require("dotenv");
const fs = require("fs");
const { start } = require("repl");
dotenv.config();

const app = express();
const port = process.env.PORT || 8080;
app.set("port", port);
app.use(morgan("dev"));
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// 프론트엔드에서 백엔드를 접근하기 위한 LIB 사용
app.use(cors());

// uploads 파일의 chart.json의 데이터 가져오기
app.get("/data", (req, res) => {
  try {
    // 받아오는 데이터 세팅 : JSON 타입으로 가져오기, 한글도 가능
    res.set("Content-Type", "application/json; charset=utf-8");
    // json을 읽을 수 있게 만들어준다. 
    const tempFile = fs.createReadStream("uploads/chart.json");
    // 응답과 json 파일을 연결시켜준다.
    return tempFile.pipe(res);
  } catch (error) {
    return res.json(error);
  }
});

app.post("/data", async (req, res) => {
  // 해당 변수명은 API에서 지정해준 변수명이므로 변경하면 ERROR 발생
  // front에서 전송하는 body안의 JSON 형식의 데이터 중 해당 변수명의 데이터를 뽑아낸다.
  const { startDate, endDate, timeUnit, device, gender, keywordGroups } = req.body;
  try {
    const request_body = {
      startDate: startDate,
      endDate: endDate,
      timeUnit: timeUnit,
      // deviec와 gender은 아무 데이터도 전송하지 않으면 "all"이란 값으로 보내온다.
      device: device === "all" ? "" : device,
      gender: gender === "all" ? "" : gender,
      keywordGroups: keywordGroups,
    };
    console.log(timeUnit);
    // 바로 위 작성한 데이터를 보내는 URL
    const url = "https://openapi.naver.com/v1/datalab/search";
    // 데이터 전송시 같이 보내는 설정
    const headers = {
      // json이라는 데이터 타입으로 전송
      "Content-Type": "application/json",
      // 해당 API의 설정 요구사항
      // .env에 작성한 변수명(CLIENT_ID = 내 ID와 CLIENT_SECRET = 내 비밀번호)
      "X-Naver-Client-Id": process.env.CLIENT_ID,
      "X-Naver-Client-Secret": process.env.CLIENT_SECRET,
    };
    // 데이터 전송 함수
    const result = await axios.post(url, request_body, {
      headers: headers,
    });

    fs.writeFile(
      `./uploads/chart.json`,
      // 받아온 데이터를 JSON 형식으로 변경하여 chart.json 파일에 작성
      JSON.stringify(result.data["results"]),
      (error) => {
        if (error) {
          throw error;
        }
      }
    );
    return res.json(result.data);
  } catch (error) {
    return res.json(error);
  }
});

app.delete("/data", (req, res) => {
  // fs.unlik : 파일을 삭제하는 명령어
  fs.unlink("uploads/chart.json", (error) => {
    if (error) {
      return res.json(error);
    }
  });
  return res.json("OK");
});

app.listen(app.get("port"), () => {
  console.log(`this server listening on ${app.get("port")}`);
});
