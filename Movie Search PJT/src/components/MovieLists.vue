<template>
  <div>
    <div class="movielists-container">
      <!-- v-for : movies를 통해 조건에 맞게 받아온 데이터들 만큼 출력하기 위한 반복 -->
        <div
        class="movie-container"
        v-for="movie in movies"
        :key="movie.id"
        @click="goDetail(movie.id)"
        >
        <!-- @click="함수" : 클릭 시 함수 실행  -->
        <!-- goDetail은 하단 지정한 methods로 해당 id를 뒤에 붙인 라우터로 이동 -->
          <div>
            <!-- 하단에서 지정한 methods인 imageParse(이미지 반환 함수)를 이용하여 포스터 보여주기 -->
            <img :src="imageParse(movie.poster_path)" width="125" height="180" alt="영화이미지">
          </div>
          <div class="movie-title">
            {{movie.title }}
          </div>
          <div class="movie-date">
            {{movie.release_date}}
          </div>
        </div>
    </div>
    
  </div>
</template>

<script>
// axios 호출
import {movieApi} from "../utils/axios"
export default {
  props:['askData', 'searchData'],
  data(){
    return {
      movies: []
    }
  },

  async created(){
    // 공통으로 사용할 컴포넌트 
    // nowPlaying,Upcomming, 데이터를 보여준다.
    // Store에서 설정해준 loading을 true로 셋팅해주기 -> 로딩 중.. 보임
    this.$store.commit("SET_LOADING", true);
    if(this.askData === "nowPlaying"){
      // 데이터를 불러오기 전
      const response = await movieApi.nowPlaying();
      // 데이터를 불러온 후
      this.movies = response.data.results;
    }
    if(this.askData === "popular"){
      const response = await movieApi.popular();
      this.movies = response.data.results;
    }
    if(this.askData === "upComing"){
      const response = await movieApi.upComing();
      this.movies = response.data.results;
    }
    // undefined인 조건에서 undefined의 length는 존재하기 않기 떄문에 에러가 발생한다.
    // this.searchData가 존재하면서 해당 배열의 길이또한 존재해야 실행하도록 작성해주었다.
    if(this.searchData && this.searchData.length){
      this.movies = this.searchData;
    }

    // Store에서 설정해준 loading을 false로 셋팅해주기 -> 로딩 중.. 안보임
    this.$store.commit("SET_LOADING", false);
  },

  methods:{
    imageParse(path){
      // TMDM에서 지정한 이미지를 가지고 오기 위한 명령어(URL)
      return `https://image.tmdb.org/t/p/w300${path}`
    },
    goDetail(id){
      // router 파일에서 설정해준 router라는 변수에 해당 URL 붙여주기
      this.$router.push(`detail/${id}`)
    }
  }
}
</script>

<style scoped>
.movielists-container{
  display: flex;
  flex-wrap: wrap;
}
.movie-container{
  width: 125px;
  margin:20px;
}
.movie-title{
  margin-top: 7px;
  font-size: 14px;
  color:#cccccc;
}
.movie-date{
  font-size: 12px;
  color:#bbbbbb;
}
.movie-container:hover{
  opacity: 0.5  ;
  cursor: pointer;
}
</style>
