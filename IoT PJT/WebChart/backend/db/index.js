const mysql = require("mysql2/promise");

const pool = mysql.createPool({
	// aws ip
	host: "",
	// mysql username
	user: "",
  // mysql user password
	password: "",
  // db name
	database: "",
	waitForConnections: true,
	connectionLimit: 10,
	queueLimit: 0,
});

module.exports = { pool };