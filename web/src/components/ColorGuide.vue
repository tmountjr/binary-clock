<template>
  <div>
    <div class="colorGuide" />
    <div class="legend">
      <span>0h/0m</span>
      <span>23h/59m</span>
    </div>
    <p>
      <em>Hours, minutes, and seconds shift color to indicate the time.</em>
    </p>
  </div>
</template>

<style scoped>
.colorGuide {
  display: block;
  height: 2rem;
  border: 1px solid rgb(var(--v-border-color));
  background: linear-gradient(
    to left,
    red 0%,
    #ff0 17%,
    lime 33%,
    cyan 50%,
    blue 66%,
    #f0f 83%,
    red 100%
  );
}

.legend {
  display: flex;
  justify-content: space-between;
}
</style>
