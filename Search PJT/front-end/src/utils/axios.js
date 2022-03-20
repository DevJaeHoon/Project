const axios = require("axios");

const api = axios.create({
  // 백엔드 URL
  baseURL: "http://localhost:8080",
});

// 해당 요청에 따라 백엔드에서 작동하는 명령어 지정

export const dataLab = {
  get: () => {
    return api.get("/data");
  },
  post: (data) => {
    return api.post("/data", data);
  },
  delete: () => {
    return api.delete("/data");
  }
}