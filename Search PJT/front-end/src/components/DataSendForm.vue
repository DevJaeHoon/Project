<template>
  <div>
    <!-- v-model은 백엔드에 설정한 변수명과 동일해야 한다. -->
    <!-- type = "date"인 input  -->
    <div class="DateMargin">
      <span class="Bold">시작일: </span> 
      <input class="DateMargin" type="date" v-model="startDate" />
      <span class="Bold">종료일: </span>  
      <input class="DateMargin" type="date" v-model="endDate" />
      <select v-model="timeUnit">
        <option value="date">일간</option>
        <option value="week">주간</option>
        <option value="month">월간</option>
      </select>
    </div>

    <!-- 원하는 그룹명과 키워드를 입력 후 추가하여 옆에 배열로 보이게 하기 -->
    <div class = "DateMargin">
      <span class="Bold">그룹명: </span>
       <input type="text" v-model="userInputGroupName" />
      <b-button
        class="DateMargin"
        v-on:click="tempGroupAdd"
        variant="outline-success"
        >추가</b-button
      >
      {{ tempGroupName }}
    </div>
    <div class="Bold DateMargin">
      키워드:
      <b-form-tags
        input-id="tags-basic"
        class="DateMargin"
        v-model="tempKeywords"
      ></b-form-tags>
    </div>

    <!-- 위에서 추가한 배열을 확정 지으면 하단에 보여주기 -->
    <div class="relative DateMargin">
      <span class="middle Bold">그룹별 키워드</span>
      <b-button
        class="RightButton DateMargin"
        v-on:click="makeGroup"
        variant="outline-success"
        >그룹 생성</b-button
      >
    </div>
    <div>
      <div v-if="keywordGroups.length">
        <div v-for="(keywordGroup, index) in keywordGroups" v-bind:key="index">
          <b-list-group-item class = "DateMargin"
            >그룹 이름: {{ keywordGroup.groupName }} <br />
            그룹 키워드: {{ keywordGroup.keywords }}</b-list-group-item
          >
        </div>
      </div>
    </div>
    <!-- 만든 데이터를 백엔드로 전송하기 -->
    <div class="relative">
      <b-button
        class="SubmitButton RightButton DateMargin"
        v-on:click="sendResultToApi"
        variant="outline-success"
        >제출</b-button
      >
    </div>
  </div>
</template>

<script>
// axios 사용
import { dataLab } from "../utils/axios";

// 초기 셋팅
export default {
  name: "DataSendForm",
  data() {
    return {
      startDate: "",
      endDate: "",
      timeUnit: "month",
      keywordGroups: [],
      userInputGroupName: "",
      tempGroupName: "",
      tempKeywords: [],
      toApiData: {},
    };
  },

  // 상단에서 사용할 함수들 작성
  methods: {
    // 그룹명 추가버튼 클릭시 좌측에 생성되는 함수
    tempGroupAdd() {
      this.tempGroupName = this.userInputGroupName;
      // input 박스 초기화
      this.userInputGroupName = "";
    },
    // 그룹 확정 버튼 클릭시 상단에서 추가한 그룹명과 키워드를 배열에 추가하는 함수
    makeGroup() {
      this.keywordGroups.push({
        groupName: this.tempGroupName,
        keywords: this.tempKeywords,
      });
      // 배열에 추가후 초기화
      this.tempGroupName = "";
      this.tempKeywords = [];
    },

    // 데이터를 백엔드로 보내기 위한 함수
    async sendResultToApi() {
      this.toApiData = {
        startDate: this.startDate,
        endDate: this.endDate,
        timeUnit: this.timeUnit,
        keywordGroups: this.keywordGroups,
      };
      console.log(this.toApiData);
      await dataLab.post(this.toApiData);
      // 화면 새로고침
      location.reload();
    },
  },
};
</script>

<style scoped>
.DateMargin {
  margin: 6px 8px;
}
.Bold{
  font-weight: bold;
  font-size: 1.3rem;
}
.relative {
  margin: 15px;
  position: relative;
  display: flex;
  align-items: center;
}
.RightButton{
  position: absolute;
  right: 20px;
}
.SubmitButton{
  margin-top: 40px;
}
</style>
