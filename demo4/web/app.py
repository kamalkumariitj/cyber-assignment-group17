from flask import Flask, request, render_template
import os
import time

app = Flask(__name__)

# Demo configuration (intentionally unsafe for controlled classroom demonstration)
# Fixed logical buffer size used in overflow simulation
BUFFER_LENGTH = 10
# Process is terminated when simulated memory grows beyond this limit
CRASH_THRESHOLD_MEMORY = 5000
# Process is terminated after this many requests (availability loss simulation)
CRASH_THRESHOLD_REQUESTS = 20

# Global state used to simulate resource exhaustion
memory_usage = []
request_count = 0


def force_exit():
    """Terminate process immediately to simulate a service crash."""
    os._exit(1)


def should_force_crash(user_input):
    """Return True when crash conditions are reached."""
    # Condition 1: accumulated memory pressure crosses threshold
    if len(memory_usage) > CRASH_THRESHOLD_MEMORY:
        return True

    # Condition 2: too many requests in a short-lived process
    if request_count > CRASH_THRESHOLD_REQUESTS:
        return True

    # Condition 3: overly long payload triggers immediate failure path
    if len(user_input) > 50:
        return True

    return False


def simulate_resource_pressure(user_input):
    """Simulate memory growth and CPU delay under larger inputs."""
    # Aggressive memory growth
    memory_usage.extend([1] * (len(user_input) * 100))

    # Deliberate delay for longer payloads
    if len(user_input) > 20:
        time.sleep(2)

    # Additional memory usage
    temp_buffer = [ord(char) for char in user_input * 10]
    memory_usage.extend(temp_buffer)


def write_to_fixed_buffer(user_input):
    """Write input into fixed-size buffer to trigger overflow behavior."""
    buffer = [0] * BUFFER_LENGTH
    # No bounds check on purpose: once i >= BUFFER_LENGTH, IndexError is raised
    for i, char in enumerate(user_input):
        buffer[i] = ord(char)
    return buffer


@app.route('/', methods=['GET', 'POST'])
def vulnerable_function():
    global request_count

    # GET request: serve demo UI
    if request.method == 'POST':
        # Track number of incoming mutation requests
        request_count += 1
        user_input = request.form.get('input', '')

        try:
            # Step 1: fail-fast crash checks (simulates unstable service behavior)
            if should_force_crash(user_input):
                force_exit()

            # Step 2: consume resources to mimic pressure under heavy input
            simulate_resource_pressure(user_input)

            # Step 3: copy user bytes into fixed buffer (overflow simulation)
            buffer = write_to_fixed_buffer(user_input)

            # Normal path for non-overflow inputs
            return f"Input processed: {''.join(map(chr, buffer))}"

        # Demonstration signal for buffer overflow condition
        except IndexError:
            return "Buffer Overflow Detected!"

        # Any unexpected runtime issue is treated as hard crash in this demo
        except Exception:
            force_exit()

    return render_template('index.html')


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=False)
