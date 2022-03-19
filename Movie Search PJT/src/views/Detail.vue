<template>
  <div class="movie-detail">

    <!-- 영화 배경 이미지 -->
    <!-- 하단에 작성한 methods를 실행하여 배경 이미지 가져오기 -->
    <div class="movie-background"
    :style="{backgroundImage: `url(${backgroundParse()}`}"
    >
    </div>

    <!-- 영화 상세 정보 -->
    <div class="movie-container">
      <!-- 포스터 이미지 -->
      <div> 
        <img class="movie-poster"
        :src="imageParse()"
        alt="영화상세정보"
         />
      </div>
        <!-- 영화 제목 -->
      <div class="movie-content">
        <h1 class="movie-title">
          {{movieDetailData.title}}
        </h1>
      <!-- 개봉일/러닝타임/장르 -->
      <div class="movie-info-wrapper"> 
        <div class="movie-open">
          개봉일: {{movieDetailData.release_date}}
        </div>
        <div>
          러닝타임: {{movieDetailData.runtime}}분
        </div>
        <div>
          <!-- 하단 지정해준 methods를 통해 3개 이하의 장르 보여주기 -->
          장르: {{ShowGenre}}
        </div>
      </div>
          <!-- 영화 소개 -->
      <div class="movie-story">
        {{movieDetailData.overview}}
      </div>

      <!-- 유튜브 -->
      <div class="movie-youtube" v-if="movieDetailData.videos.results.length"></div>
        <!-- 하단에 작성한 methods를 이용하여 키 가져와 키에 맞는 영상 보여주기 -->
        <iframe
        width="560"
        height="315"
        frameborder="0"
        allowfullscreen
        :src="youtubeParse()"
        >
        </iframe>
      </div>
    </div>
  </div>
</template>

<script>
// api 가져오기
import {movieApi} from "../utils/axios"

export default {
  data(){
    return {
      movieDetailData:{},
      ShowGenre:""
    }
  },
  async created(){
    this.$store.commit("SET_LOADING", true); // 로딩
    // id를 가져와서 id에 맞는 데이터의 세부 사항을 출력해주기 위함
    const response = await movieApi.movieDetail(this.$route.params.id);
    this.movieDetailData = response.data;
    console.log(this.movieDetailData);
    this.genreParse();
    this.$store.commit("SET_LOADING", false); // 로딩 종료
  },
  methods:{
    imageParse(){
      // 포스터 이미지 가져오기
      return `https://image.tmdb.org/t/p/original/${this.movieDetailData.poster_path}`
    },
    backgroundParse(){
      // 배경 포스터 이미지 가져오기
      return `https://image.tmdb.org/t/p/original/${this.movieDetailData.backdrop_path}`;
    },
    // 영화를 소개하는 여러 영상 데이터 중 0번째 영상 데이터 키 가져오기
    youtubeParse(){
      return `https://www.youtube.com/embed/${this.movieDetailData.videos.results[0].key}`
    },
    genreParse(){
      const genres = this.movieDetailData.genres;
      // 해당 영화의 장르를 3개 이하만 가져오기 위해서
      for(let i=0; i<genres.length && i < 3; i++) {
        this.ShowGenre += genres[i].name + ", ";
      }
      // 마지막 장르에 붙은 ,(콤마)를 없애주기 위해
      this.ShowGenre = this.ShowGenre.substring(0, this.ShowGenre.length-2);
    }
  }
}
</script>


<style scoped>
/* scoped - 해당 컴포넌트에서만 해당 CSS를 적용하는 제한 */
.movie-detail {
  position: relative;
  margin-top: 50px;
}
.movie-background {
  background-size: cover;
  height: 100vh;
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  z-index: 0;
}
.movie-background::after {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  min-height: 100vh;
  background-color: rgb(40, 40, 40);
  opacity: 0.8;
  content: "";
  display: block;
}
.movie-container {
  position: relative;
  z-index: 999;
  display: flex;
}
.movie-content {
  margin-left: 25px;
  color: white;
}
.movie-poster {
  height: 80vh;
  margin-left: 40px;
}
.movie-info-wrapper{
  display: flex;
  align-items: center;
  margin-top: 20px;
}
.movie-info-wrapper>div{

  font-size: 14px;
  color:#eeeeeeee;
}
.movie-info-wrapper>div:not(:first-child){
  margin-left: 15px;
}
.movie-info-wrapper a{
  color:#eeeeee;
  font-size: 15px;
}
.movie-story {
  margin-top: 20px;
  max-width: 33vw;
  color: #bbbbbb;
}
.movie-youtube {
  margin-top: 40px;
}
</style>
