import { createApp } from "vue";
import App from "./App.vue";

// Vuetify
import { createVuetify } from "vuetify";
import {
  VApp,
  VNavigationDrawer,
  VList,
  VListSubheader,
  VListItem,
  VAppBar,
  VAppBarNavIcon,
  VToolbarTitle,
  VBtn,
  VMain,
  VContainer,
  VRow,
  VCol,
  VCard,
  VCardItem,
  VCardTitle,
  VCardActions,
  VSlider,
} from "vuetify/components";
import { aliases, mdi } from "vuetify/iconsets/mdi";
const vuetify = createVuetify({
  icons: {
    defaultSet: "mdi",
    aliases,
    sets: { mdi },
  },
  components: {
    VApp,
    VNavigationDrawer,
    VList,
    VListSubheader,
    VListItem,
    VAppBar,
    VAppBarNavIcon,
    VToolbarTitle,
    VBtn,
    VMain,
    VContainer,
    VRow,
    VCol,
    VCard,
    VCardItem,
    VCardTitle,
    VCardActions,
    VSlider,
  },
});

import "./assets/main.css";

const app = createApp(App);

app.use(vuetify);
app.mount("#app");
