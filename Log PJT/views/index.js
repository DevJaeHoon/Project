// 각 메시지 내용에 맞게 배경색 바꿔주기
const changeDOM = () => {
    const messagArea = document.querySelectorAll(".messageArea");
    messagArea.forEach(element => {
        // 보안성 취약으로 innerHTML은 주로 안쓰지만, 편리하다
        if(element.innerHTML.includes("warn")) {
            element.classList.remove("alert-primary");
            element.classList.add("alert-warning");
        }
        if(element.innerHTML.includes("info")) {
            element.classList.remove("alert-primary");
            element.classList.add("alert-info");
        }
        if(element.innerHTML.includes("error")) {
            element.classList.remove("alert-primary");
            element.classList.add("alert-danger");
        }
    })
}

changeDOM();