const ignoredKeys = ['Shift', 'Control', 'Alt', 'Meta'];

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
    });
});