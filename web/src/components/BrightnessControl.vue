<template>
  <v-col cols="12">
    <v-card>
      <v-card-item>
        <v-card-title>Brightness</v-card-title>
      </v-card-item>
      <v-card-text>
        <p>Set the LED brightness.</p>
        <v-slider
          prepend-icon="mdi-weather-sunny-off"
          append-icon="mdi-weather-sunny"
          v-model="brightness"
          @click:prepend="brightnessOff"
          @click:append="brightnessFull"
          min="1"
          max="100"
          step="1"
        />
        <p>Current device setting: {{ initialBrightness }}</p>
        <p>New setting: {{ brightness }}</p>
      </v-card-text>
      <v-card-actions>
        <v-btn prepend-icon="mdi-content-save-outline" @click="saveBrightness">
          Save
        </v-btn>
      </v-card-actions>
    </v-card>
  </v-col>
</template>

<script>
export default {
  data: () => ({
    brightness: 0,
    initialBrightness: 0,
  }),
  mounted() {
    this.getDeviceBrightness();
  },
  methods: {
    getDeviceBrightness() {
      console.debug("TODO: implement brightness fetch");
      this.initialBrightness = 25;
      this.brightness = 25;
    },
    brightnessOff() {
      this.brightness = 0;
    },
    brightnessFull() {
      this.brightness = 100;
    },
    saveBrightness() {
      console.debug("TODO: implement brightness route and functionality");
      // For now, replicate the functionality by setting the values.
      // Eventually we'll post and get the response back.
      this.initialBrightness = this.brightness;
    },
  },
};
</script>
