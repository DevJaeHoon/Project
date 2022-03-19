<template>
  <div id="app">
    <!-- <div id="nav"> -->
      <nav class="move-nav">
        <router-link to="/">Home</router-link> |
        <router-link to="/search">Search</router-link>
      </nav>
      
    <!-- </div> -->
    <!-- loding이 true인 경우에만 보여주면된다 -->
    <!-- Bootstrap을 이용하여 로딩(버퍼링) 보여주기 -->
    <b-spinner v-if="$store.state.loading" class="loading-icon"></b-spinner>
    <router-view/>
  </div>
</template>

<style>
/* 외부 폰트 가져오기 */
@import url('https://fonts.googleapis.com/css2?family=Noto+Sans+KR:wght@100;300;400&display=swap');

/* 모든 페이지, 모든 라우터에 적용될 CSS */
#app {
  font-family: 'Noto Sans KR', sans-serif;
  /* font-family: Avenir, Helvetica, Arial, sans-serif; */
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  background-color: rgb(20, 20, 20);
  color: white;
  min-height: 100vh;
}

nav{
  position: relative;
  z-index: 999;
  padding: 20px;
  background-color: black;
}

nav a{
  font-weight: bold;
  color:#2c3e50;
}

nav a:hover{
  color: white;
}
/* router-link-exact-active : router-link가 활성화 됐을때 지정된 class */
.router-link-exact-active{
  color: white;
}

.loading-icon{
  margin: 50px;
}
</style>
