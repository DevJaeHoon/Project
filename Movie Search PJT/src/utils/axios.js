import axios from "axios";

//공통으로 들어가는 URL
const DOMAIN = "https://api.themoviedb.org/3";

// axios 기본 셋팅
const request = axios.create({
  // request로 요청하는 모든 요청이 기본적으로 앞에 baseURL이 탑재가 된다.
  // baseUrl -> baseURL
  baseURL: DOMAIN,
  params: {
    api_key: "d70e44a25c7b2738d37503b18372a99b",
    language: "ko-KR"
  }
});

// TMDB에서 요구하는 명령어들을 사용함
export const movieApi = {
  nowPlaying: () => {return request.get("/movie/now_playing")},
  popular: () => {return request.get("/movie/popular")},
  upComing: () => {return request.get("/movie/upcoming")},
  movieDetail: (movie_id) => {
    // /movie/{movie_id}/videos 호출하면 /movie/{movie_id}와 /movie/{movie_id}/videos가 각각 한번씩 두번 호출한다.
    // 하지만, append_to_response를 활용하면 /movie/{movie_id}/videos만 호출
    return request.get(`/movie/${movie_id}`, {
      params: {
        append_to_response:"videos"
      }
    })
  },
  search: (keyword) => {return request.get("/search/movie", {
    params:{
      query:keyword
    }
  })}
}
