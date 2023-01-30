<template>
  <v-col cols="12" md="4">
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
const GET_BRIGHTNESS_URL = "/brightness";
const SET_BRIGHTNESS_URL = `${GET_BRIGHTNESS_URL}/set`;

export default {
  data: () => ({
    brightness: 0,
    initialBrightness: 0,
  }),
  async mounted() {
    await this.getDeviceBrightness();
    this.brightness = this.initialBrightness;
  },
  methods: {
    async getDeviceBrightness() {
      try {
        const resp = await fetch(GET_BRIGHTNESS_URL);
        const data = await resp.json();
        this.initialBrightness = data.brightness;
      } catch (e) {
        console.error(e);
      }
    },
    brightnessOff() {
      this.brightness = 1;
    },
    brightnessFull() {
      this.brightness = 100;
    },
    async saveBrightness() {
      try {
        const resp = await fetch(SET_BRIGHTNESS_URL, {
          method: "POST",
          body: JSON.stringify({
            brightness: this.brightness,
          }),
          headers: {
            Accept: "application/json",
            "Content-Type": "application/json",
          },
        });
        const data = await resp.json();
        this.initialBrightness = data.brightness;
      } catch (e) {
        console.error(e);
      }
    },
  },
};
</script>
