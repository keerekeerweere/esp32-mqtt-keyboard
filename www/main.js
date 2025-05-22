const ignoredKeys = ['Shift', 'Control', 'Alt', 'Meta'];

document.addEventListener("DOMContentLoaded", () => {
    const statusEl = document.getElementById("status");

    // Handle actual user keypresses
    document.addEventListener('keydown', async (e) => {
        const fnKey = e.key.match(/^F(\d{1,2})$/);
        if (fnKey && +fnKey[1] <= 12) {
            e.preventDefault();
            return;
        }

        if (ignoredKeys.includes(e.key)) return;

        const data = {
            key: e.key,
            location: e.location, // 0 = standard, 1 = left, 2 = right, 3 = numpad
            ctrlKey: e.ctrlKey,
            shiftKey: e.shiftKey,
            altKey: e.altKey,
            metaKey: e.metaKey
        };

        try {
            await fetch('/keypress', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(data)
            });
            statusEl.textContent = `Key sent: ${e.key}`;
        } catch (err) {
            console.error('Failed to send keypress:', err);
            statusEl.textContent = 'Error sending key';
        }
    });

    // Simulate exact keypresses for command buttons
    document.querySelectorAll('.control-button').forEach(button => {
        button.addEventListener('click', async () => {
            const keyCombo = button.getAttribute('data-key');
            let data;

            switch (keyCombo) {
                case 'Ctrl+C':
                    data = {
                        key: 'c',
                        location: 1,
                        ctrlKey: true,
                        shiftKey: false,
                        altKey: false,
                        metaKey: false
                    };
                    break;
                case 'Ctrl+D':
                    data = {
                        key: 'd',
                        location: 1,
                        ctrlKey: true,
                        shiftKey: false,
                        altKey: false,
                        metaKey: false
                    };
                    break;
                case 'CapsLock':
                    data = {
                        key: 'CapsLock',
                        location: 0,
                        ctrlKey: false,
                        shiftKey: false,
                        altKey: false,
                        metaKey: false
                    };
                    break;
                case 'Ctrl+Alt+Delete':
                    data = {
                        key: 'Delete',
                        location: 1,
                        ctrlKey: true,
                        altKey: true,
                        shiftKey: false,
                        metaKey: false
                    };
                    break;
                case 'Alt+F4':
                    data = {
                        key: 'F4',
                        location: 1,
                        ctrlKey: false,
                        shiftKey: false,
                        altKey: true,
                        metaKey: false
                    };
                    break;
                case 'Ctrl+Z':
                    data = {
                        key: 'z',
                        location: 1,
                        ctrlKey: true,
                        shiftKey: false,
                        altKey: false,
                        metaKey: false
                    };
                    break;
                default:
                    return;
            }

            try {
                await fetch('/keypress', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(data)
                });
                statusEl.textContent = `Command sent: ${keyCombo}`;
            } catch (err) {
                console.error('Failed to send command:', err);
                statusEl.textContent = 'Error sending command';
            }
        });
    });
});
