document.addEventListener('DOMContentLoaded', () => {
    const API_BASE_URL = 'http://localhost:9000';

    const generateForm = document.getElementById('generate-form');
    const generateResult = document.getElementById('generate-result');

    const composeForm = document.getElementById('compose-form');
    const composeResult = document.getElementById('compose-result');
    const composeComponentsResult = document.getElementById('compose-components');
    const toggleComponentsBtn = document.getElementById('toggle-components-btn');

    const errorArea = document.getElementById('error-area');

    const tabLinks = document.querySelectorAll('.tab-link');
    const tabPanes = document.querySelectorAll('.tab-pane');

    const showError = (message) => {
        errorArea.textContent = `Ошибка: ${message}`;
        errorArea.style.display = 'block';
        console.error(message);
    };

    const clearError = () => {
        errorArea.textContent = '';
        errorArea.style.display = 'none';
    };

    const fetchData = async (url) => {
        clearError();
        try {
            const response = await fetch(url);

            if (!response.ok) {
                let errorMsg = `HTTP ошибка ${response.status} (${response.statusText})`;
                try {
                    const errorData = await response.json();
                    errorMsg += ` - ${JSON.stringify(errorData)}`;
                } catch (e) {

                }
                 throw new Error(errorMsg);
            }

            return await response.json();

        } catch (error) {
            showError(`Не удалось получить данные. ${error.message}. Убедитесь, что бэкенд запущен на ${API_BASE_URL} и доступен.`);
            return null;
        }
    };

    tabLinks.forEach(link => {
        link.addEventListener('click', () => {
            const targetTabId = link.getAttribute('data-tab');

            tabLinks.forEach(l => l.classList.remove('active'));
            tabPanes.forEach(p => p.classList.remove('active'));

            link.classList.add('active');
            document.getElementById(targetTabId).classList.add('active');
            clearError();
        });
    });

    if (toggleComponentsBtn && composeComponentsResult) {
         let componentsVisible = true;
         composeComponentsResult.classList.remove('hidden');
         toggleComponentsBtn.textContent = 'Скрыть компоненты';


        toggleComponentsBtn.addEventListener('click', () => {
            componentsVisible = !componentsVisible;
            if (componentsVisible) {
                composeComponentsResult.classList.remove('hidden');
                toggleComponentsBtn.textContent = 'Скрыть компоненты';
            } else {
                composeComponentsResult.classList.add('hidden');
                toggleComponentsBtn.textContent = 'Показать компоненты';
            }
        });
    }


    if (generateForm) {
        generateForm.addEventListener('submit', async (event) => {
            event.preventDefault();
            generateResult.textContent = 'Генерация...';

            const formData = new FormData(generateForm);
            const gender = formData.get('generate_gender');
            const excuseType = formData.get('generate_excuse_type');

            const url = new URL(`${API_BASE_URL}/generate`);
            url.searchParams.append('gender', gender);
            url.searchParams.append('excuse_type', excuseType);

            const data = await fetchData(url.toString());

            if (data && data.excuse) {
                generateResult.textContent = data.excuse;
            } else if (!errorArea.style.display || errorArea.style.display === 'none') {
                generateResult.textContent = 'Не удалось сгенерировать отмазку.';
            } else {
                generateResult.textContent = 'Произошла ошибка (см. выше).';
            }
        });
    }


    if (composeForm) {
        composeForm.addEventListener('submit', async (event) => {
            event.preventDefault();
            composeResult.textContent = 'Составление...';

            if (composeComponentsResult.classList.contains('hidden')) {
                 componentsVisible = true;
                 composeComponentsResult.classList.remove('hidden');
                 toggleComponentsBtn.textContent = 'Скрыть компоненты';
            }
            composeComponentsResult.textContent = 'Загрузка компонентов...';


            const formData = new FormData(composeForm);
            const params = new URLSearchParams();

            params.append('gender', formData.get('compose_gender'));
            params.append('excuse_type', formData.get('compose_excuse_type'));

            const componentFields = ['opening', 'cause', 'incident', 'action', 'object', 'place', 'consequence'];
            componentFields.forEach(field => {
                const value = formData.get(field);
                if (value && value.trim() !== '') {
                    params.append(field, value.trim());
                }
            });

            const url = `${API_BASE_URL}/compose?${params.toString()}`;

            const data = await fetchData(url);

            if (data && data.excuse && data.components) {
                composeResult.textContent = data.excuse;
                composeComponentsResult.textContent = JSON.stringify(data.components, null, 2);
            } else if (!errorArea.style.display || errorArea.style.display === 'none'){
                composeResult.textContent = 'Не удалось составить отмазку.';
                composeComponentsResult.textContent = '-';
            } else {
                composeResult.textContent = 'Произошла ошибка (см. выше).';
                composeComponentsResult.textContent = '-';
            }
        });
    }
});