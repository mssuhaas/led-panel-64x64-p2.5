const sendButton = document.getElementById('sendButton');
const responseMessage = document.getElementById('responseMessage');

sendButton.addEventListener('click', async () => {
    console.log('Button clicked');

    const serverIP = document.getElementById('serverIP').value;
    const jsonData = document.getElementById('jsonData').value;

    console.log('Server IP:', serverIP);
    console.log('JSON Data:', jsonData);

    if (!serverIP || !jsonData) {
        responseMessage.textContent = 'Please fill in both IP and JSON data.';
        return;
    }

    const jsonDataNew = { data: jsonData };  // Create an object for the JSON data

    try {
        console.log('Sending POST request...');

        const response = await axios.post(`http://${serverIP}:8100`, jsonDataNew, {
            headers: {
                'Content-Type': 'application/json'
            }
        });

        console.log('Response received:', response.data);

        responseMessage.textContent = `Response from server: ${JSON.stringify(response.data)}`;
    } catch (error) {
        console.error('An error occurred:', error);

        responseMessage.textContent = 'An error occurred while sending the request.';
    }
});
