<template>
  <v-app :theme="theme">
    <v-navigation-drawer v-model="drawer">
      <v-list v-model:opened="open">
        <v-list-subheader inset>Diagnostics</v-list-subheader>
        <v-list-item prepend-icon="mdi-palette" @click="cycleColors">
          Cycle Colors
        </v-list-item>

        <v-list-subheader inset>Help</v-list-subheader>
        <v-list-group value="How to Read">
          <template v-slot:activator="{ props }">
            <v-list-item
              v-bind="props"
              prepend-icon="mdi-help-circle-outline"
              title="How to Read"
            />
          </template>

          <v-list-item>
            <ColorGuide />
          </v-list-item>
          <v-list-item>
            <BinaryGuide />
          </v-list-item>
        </v-list-group>
      </v-list>
    </v-navigation-drawer>
    <v-app-bar>
      <v-app-bar-nav-icon @click="drawer = !drawer"></v-app-bar-nav-icon>
      <v-toolbar-title>Binary Clock</v-toolbar-title>
      <v-btn :icon="themeIcon" @click="onClick" />
    </v-app-bar>

    <v-main>
      <v-container fluid>
        <v-row dense>
          <DeviceTime />
          <BrightnessControl />
          <InvertDisplay />
        </v-row>
      </v-container>
    </v-main>
  </v-app>
</template>

<script>
import BrightnessControl from "./components/BrightnessControl.vue";
import DeviceTime from "./components/DeviceTime.vue";
import InvertDisplay from "./components/InvertDisplay.vue";
import ColorGuide from "./components/ColorGuide.vue";
import BinaryGuide from "./components/BinaryGuide.vue";

export default {
  data: () => ({
    theme: "light",
    drawer: false,
    open: ["How to Read"],
  }),
  methods: {
    onClick() {
      this.theme = this.theme === "light" ? "dark" : "light";
    },
    cycleColors() {
      console.debug("TODO: implement cycle color route and functionality");
    },
  },
  computed: {
    themeIcon() {
      return this.theme === "light" ? "mdi-weather-sunny" : "mdi-weather-night";
    },
  },
  components: {
    DeviceTime,
    InvertDisplay,
    BrightnessControl,
    ColorGuide,
    BinaryGuide,
  },
};
</script>
