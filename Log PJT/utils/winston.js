const winston = require("winston");
const { format } = require("winston");
// format.combine 으로 사용해도 무관함
const { combine } = format;

// {a} 와 a의 차이는 
// {a}는 가져오려는 lib에서 a만 가져오기
// a에 lib 전체를 가져오기
// log에 찍히는 시간을 직관적으로 하기 위해 T~~ -> 14:10:33
const moment = require("moment");

require("winston-daily-rotate-file");

const transport = new winston.transports.DailyRotateFile({
  level: "info",
  filename: "./logs/%DATE%.log",
  datePattern: "YYYY-MM--DD--HH",
  zippedArchive: true,
  maxSize: "20m",
  maxFiles: "1d",
  format: combine(
    format.timestamp({ format: moment().format("YYYY-MM-DD HH:mm:ss") }),
    format.json()
  ),
});

const logger = winston.createLogger({
  transports: [transport],
});

module.exports = { logger };
