<script setup>
import { ref } from 'vue'

// Khai báo các biến reactive
const deviceStatus = ref(false) // Trạng thái ban đầu là tắt
const dataValue = ref(50) // Giá trị mẫu cho thanh tiến trình
const deviceIp = ref('') // IP của thiết bị IoT (ban đầu rỗng)
const defaultIp = '192.168.1.100' // IP mặc định (có thể thay đổi)

const toggleDevice = () => {
  // Logic giả lập gửi lệnh đến thiết bị IoT qua IP đã nhập (qua WebSocket, MQTT, hoặc API)
  if (!deviceIp.value) {
    alert('Vui lòng nhập IP của thiết bị IoT!')
    return
  }
  deviceStatus.value = !deviceStatus.value
  console.log(`Gửi lệnh đến ${deviceIp.value}:`, deviceStatus.value ? 'Bật' : 'Tắt')
  // Ở đây bạn có thể gọi API hoặc WebSocket/MQTT với IP của thiết bị
  // Ví dụ: gửi lệnh qua WebSocket hoặc MQTT đến `deviceIp.value`
}

const saveIp = () => {
  if (!deviceIp.value) {
    alert('Vui lòng nhập IP hợp lệ!')
    return
  }
  console.log(`Đã lưu IP thiết bị: ${deviceIp.value}`)
  // Có thể thêm logic để kiểm tra IP hợp lệ (regex hoặc API)
}
</script>

<template>
  <div class="min-h-screen bg-gray-100 flex flex-col items-center justify-center p-4">
    <!-- Header -->
    <header class="w-full max-w-md bg-blue-600 text-white p-4 rounded-lg shadow-md mb-6">
      <h1 class="text-2xl font-bold text-center">Điều Khiển Thiết Bị IoT</h1>
    </header>

    <!-- Device Control Card -->
    <div class="w-full max-w-md bg-white p-6 rounded-lg shadow-md">
      <!-- Phần nhập IP -->
      <div class="mb-6">
        <label for="deviceIp" class="block text-sm font-medium text-gray-700 mb-2">Địa chỉ IP của thiết bị IoT:</label>
        <div class="flex items-center gap-2">
          <input
              id="deviceIp"
              v-model="deviceIp"
              type="text"
              placeholder="Nhập IP (e.g., 192.168.1.100)"
              class="w-full p-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
          />
          <button
              @click="saveIp"
              class="bg-blue-500 text-white w-1/4 py-2 rounded-lg hover:bg-blue-600 transition-colors duration-300"
          >
            Lưu IP
          </button>
        </div>
        <p v-if="!deviceIp" class="text-sm text-red-500 mt-1">IP chưa được nhập!</p>
        <p v-else class="text-sm text-gray-600 mt-1">IP hiện tại: {{ deviceIp }}</p>
      </div>

      <h2 class="text-xl font-semibold mb-4 text-gray-800">Trạng Thái Đèn LED</h2>
      <div class="flex items-center justify-between mb-4">
        <span class="text-gray-600">Bật/Tắt:</span>
        <button
            @click="toggleDevice"
            :class="['px-4 py-2 rounded-full', deviceStatus ? 'bg-green-500 text-white' : 'bg-red-500 text-white']"
        >
          {{ deviceStatus ? 'Bật' : 'Tắt' }}
        </button>
      </div>
      <p class="text-gray-700">Trạng thái hiện tại: {{ deviceStatus ? 'Bật' : 'Tắt' }}</p>

      <!-- Real-time Data (Progress Bar) -->
      <div class="mt-6">
        <h3 class="text-lg font-semibold mb-2 text-gray-800">Dữ Liệu Thời Gian Thực</h3>
        <div class="w-full bg-gray-200 rounded-full h-4">
          <div
              class="bg-blue-500 h-4 rounded-full"
              :style="{ width: `${dataValue}%` }"
          ></div>
        </div>
        <p class="text-gray-600 mt-2">Giá trị: {{ dataValue }}%</p>
      </div>
    </div>
  </div>
</template>

<style scoped>
/* Thêm style tùy chỉnh nếu cần, nhưng Tailwind đã xử lý phần lớn */
header {
  line-height: 1.5;
}

@media (min-width: 1024px) {
  header {
    display: flex;
    place-items: center;
    padding-right: calc(var(--section-gap) / 2);
  }
}
</style>