<template>
<!-- v-if : loading이 false일때만 컴포넌트를 컸다 키도록 하기 위해-->
  <div v-if="!$store.state.loading">
    <!-- @keyup.enter="함수" : 키보드로 Enter키 누르면 , 함수가 실행된다. -->
    <input
      v-model="inputText"
      @keyup.enter="searchMovie"
     class="movie-search" placeholder="영화 제목을 입력하세요.">
    
     <!-- Seach페이지에서 데이터를 검색한후-> 해당 데이터를 props로 넘겨준다. -->
     <!-- v-if : searchData에 데이터가 들어온다면 실행  -->
     <!-- 문자열을 넘기는게 아닌 searchData에 들어온 데이터를 props를 통해 넘기는것 이므로 v-bind(:) 사용 -->
    <MovieLists v-if="searchData.length" :searchData="searchData"/>
  </div>
</template>

<script>
import MovieLists from "../components/MovieLists.vue"
import { movieApi } from '../utils/axios';

export default {
  components:{
    MovieLists
  },
  data(){
    return {
      inputText: "",
      searchData: []
    }
  },
  created(){
    // search 할 때 로딩이 되므로, 처음에는 로딩 꺼주기!
    this.$store.commit("SET_LOADING", false);
  },
  methods:{
    async searchMovie(){
      // v-model의 데이터를 받아서 검색을하고 그결과를 props로 넘겨준다.
      this.$store.commit("SET_LOADING", true); // 로딩하기!
      // axios에서 설정한 search는 "keyword"라는 매개변수 필요!
      const response = await movieApi.search(this.inputText);
      this.searchData = response.data.results;
      this.$store.commit("SET_LOADING", false); // 다시 로딩 끄기!
    }
  }
  
}
</script>

<style>
.movie-search{
  display: block;
  width:100%;
  padding:12px;
}
</style>
