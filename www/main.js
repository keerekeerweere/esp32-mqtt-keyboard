const ignoredKeys = ['Shift', 'Control', 'Alt', 'Meta'];

const keyBindings = {
    'Control+Alt+Delete': 'Ctrl+Alt+Delete',
    'Control+c': 'Ctrl+C',
    'Control+d': 'Ctrl+D',
    'CapsLock': 'CapsLock'
};

document.addEventListener("DOMContentLoaded", () => {
    const statusEl = document.getElementById("status");

    document.addEventListener('keydown', async (e) => {
        const fnKey = e.key.match(/^F(\d{1,2})$/);
        if (fnKey && +fnKey[1] <= 12) {
            e.preventDefault();
            return;
        }

        if (ignoredKeys.includes(e.key)) return;

        const data = {
            key: e.key,
            location: e.location,
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

        // Check for defined shortcuts (except Ctrl+C to allow native copy)
        const combo = [
            e.ctrlKey ? 'Control' : '',
            e.altKey ? 'Alt' : '',
            e.shiftKey ? 'Shift' : '',
            e.metaKey ? 'Meta' : '',
            e.key
        ].filter(Boolean).join('+');

        if (keyBindings[combo] && combo !== 'Control+c') {
            e.preventDefault();
            triggerCommand(keyBindings[combo]);
        }
    });

    // Button click event handler
    document.querySelectorAll('.control-button').forEach(button => {
        button.addEventListener('click', () => {
            const keyCombo = button.getAttribute('data-key');
            triggerCommand(keyCombo);
        });
    });

    async function triggerCommand(keyCombo) {
        const parts = keyCombo.split('+');
        const key = parts.pop();

        const data = {
            key: key,
            location: 0,
            ctrlKey: parts.includes('Ctrl'),
            altKey: parts.includes('Alt'),
            shiftKey: parts.includes('Shift'),
            metaKey: parts.includes('Meta')
        };

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
    }
});