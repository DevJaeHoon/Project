const express = require("express");
const morgan = require("morgan");

const { pool } = require("./db");

const app = express();
const PORT = 8081;
app.use(morgan("dev"));
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
const cors = require("cors");
app.use(
	cors({
		origin: true,
	})
);

const server = require("http").createServer(app);
const io = require("socket.io")(server, {
	// 연결이 끊어졌음을 판단하는 기준 밀리초
	pingTimeout: 1,
	// 새 패킷 보내기까지의 밀리초
	pingInterval: 100,
});
io.on("connection", async (socket) => {
	console.log("user connected");
	try {
		// sensing 테이블의 모든 데이터 가져오기
		const data = await pool.query("select * from sensing");
		// 프론트에서 getData 요청을 하면 
		// 데이터를 보낸다!
		socket.emit("getData", data[0]);
	} catch (error) {
		console.log(error);
	}
});

// 반드시, app.listen 이 아니라 server.listen 으로 지정해야.
// 왜? socket.io 의 포트 배정이고, 소켓은 app 이 아닌 server 이기 때문
server.listen(PORT, () => console.log(`this server listening on ${PORT}`));