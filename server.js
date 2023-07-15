const express = require('express');
const bodyParser = require('body-parser');
const app = express();
const path = require('path');

app.use(bodyParser.json());

let formData = null; // Variable to store form data

app.post('/login', (req, res) => {
  const { username, password } = req.body;

  formData = { username, password }; // Store the form data

  res.sendStatus(200); // Send a success response to the client
});

app.get('/getdata', (req, res) => {
  res.json(formData); // Send the stored form data as the response
});

// Serve the login page for the root route
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

const port = 3000; // Set your desired port number
app.listen(port, () => {
  console.log(`Server is running on port ${port}`);
});
