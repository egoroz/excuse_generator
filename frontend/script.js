document.addEventListener('DOMContentLoaded', function() {
    const generateButton = document.getElementById('generateButton');
    const excuseText = document.getElementById('excuseText');
    const genderSelect = document.getElementById('gender'); 
    const excuseTypeSelect = document.getElementById('excuseType');

    generateButton.addEventListener('click', function() {
        const gender = genderSelect.value;
        const excuse_type = excuseTypeSelect.value;

        const url = `http://localhost:1234/generate?gender=${gender}&excuse_type=${excuse_type}`;

        fetch(url)
            .then(response => {
                if (!response.ok) {
                    throw new Error(`HTTP error! Status: ${response.status}`);
                }
                return response.json();
            })
            .then(data => {
                excuseText.textContent = data.excuse;
            })
            .catch(error => {
                console.error('Ошибка:', error);
                excuseText.textContent = `Произошла ошибка при получении оправдания: ${error}`;
            });
    });
});