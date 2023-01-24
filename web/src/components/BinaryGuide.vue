<template>
  <div>
    <div class="binaryGuide">
      <span>1</span>
      <span>2</span>
      <span>4</span>
      <span>8</span>
      <span>16</span>
      <span>32</span>
    </div>
    <p>
      <em>
        From bottom to top, each LED represents a binary bit. To find the number
        shown by the LEDs, add up the values of each light LED.
      </em>
    </p>
  </div>
</template>

<style scoped>
.binaryGuide {
  display: block;
  height: 2rem;
  display: flex;
  justify-content: space-between;
  align-items: center;
}

span {
  border: 1px solid rgb(var(--v-border-color));
  flex-grow: 1;
  text-align: center;
}
</style>
