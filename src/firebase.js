// src/firebase.js
import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database";

// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyC6Y1_5jSQp4hLLtaUn9ApWvRm34qtClUI",             // Same as Arduino
  authDomain: "cpc357-6fbdf.firebaseapp.com",
  databaseURL: "https://cpc357-6fbdf-default-rtdb.asia-southeast1.firebasedatabase.app", // Same as Arduino (but with https://)
  projectId: "cpc357-6fbdf",
  storageBucket: "cpc357-6fbdf.firebasestorage.app",
  messagingSenderId: "323589900950",
  appId: "1:323589900950:web:38485a959def08aae650b7"
};

// 1. Initialize Firebase
const app = initializeApp(firebaseConfig);

// 2. Initialize Realtime Database and get a reference to the service
const db = getDatabase(app);

// 3. EXPORT the database so App.js can find it!
export { db };