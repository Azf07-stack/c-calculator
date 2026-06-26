const screen = document.querySelector('.calculator-screen');
const buttons = document.querySelectorAll('button');

async function sendToBackend(n1, opCode, n2) {
    // 1. DELETE the old localhost line.
    // 2. PASTE your Render URL here. 
    // IMPORTANT: Do NOT put a slash (/) at the very end of the URL!
    const backendBaseUrl = 'https://c-calculator.onrender.com'; 
    
    const serverURL = `${backendBaseUrl}/?n1=${n1}&op=${opCode}&n2=${n2}`;
    
    try {
        const response = await fetch(serverURL);
        const answer = await response.text();
        screen.innerText = answer;
    } catch (error) {
        console.error("Network Error:", error);
        screen.innerText = "Server Error";
    }
}

// ... the rest of your button logic stays exactly the same ...

// REFACTOR 2: Our button logic is much shorter and easier to read now.
buttons.forEach(button => {
    button.addEventListener('click', () => {
        const buttonText = button.innerText;

        if (buttonText === 'C') {
            screen.innerText = '0';
            return; 
        }

        if (buttonText === 'DEL') {
            if (screen.innerText.length > 1) {
                screen.innerText = screen.innerText.slice(0, -1);
            } else {
                screen.innerText = '0';
            }
            return;
        }

        if (buttonText === '=') {
            const equation = screen.innerText;
            screen.innerText = "Calculating..."; 

            let opCode = '';
            let opChar = '';
            
            if (equation.includes('+')) { opChar = '+'; opCode = 'p'; }
            else if (equation.includes('-')) { opChar = '-'; opCode = 'm'; }
            else if (equation.includes('*')) { opChar = '*'; opCode = 'x'; }
            else if (equation.includes('/')) { opChar = '/'; opCode = 'd'; }

            if (opCode !== '') {
                const parts = equation.split(opChar);
                // Call our new clean function here!
                sendToBackend(parts[0], opCode, parts[1]);
            } else {
                screen.innerText = "Error"; 
            }
            return;
        }

        if (screen.innerText === '0') {
            screen.innerText = buttonText;
        } else {
            screen.innerText = screen.innerText + buttonText;
        }
    });
});