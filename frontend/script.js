document.addEventListener('DOMContentLoaded', function() {
    // Этот код выполнится, когда HTML документ будет полностью загружен

    const generateButton = document.getElementById('generateButton'); // Находим кнопку по ID
    const excuseText = document.getElementById('excuseText'); // Находим абзац для текста

    generateButton.addEventListener('click', function() {
        // Этот код выполнится при нажатии на кнопку

        // Отправляем GET запрос к API
      
        fetch('http://localhost:1234/generate')
        .then(response => {
             if (!response.ok) { // Проверяем HTTP статус
                    throw new Error(`HTTP error! Status: ${response.status}`);
              }
             return response.json(); // Преобразуем ответ в JSON
         })
        .then(data => {
            excuseText.textContent = data.excuse; // Вставляем оправдание в абзац
        })
        .catch(error => {
            console.error('Ошибка:', error); // Выводим сообщение об ошибке в консоль
            excuseText.textContent = `Произошла ошибка при получении оправдания: ${error}`; // Добавляем детали ошибки на страницу
        });


    });
});