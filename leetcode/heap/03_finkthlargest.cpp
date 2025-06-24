class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int n = nums.size();
        return quickSelect(nums, 0, n - 1, n - k); // 第k大的数 == 第(n-k)小
    }


    int quickSelect(vector<int>& nums, int left, int right, int k_smallest) {
        if (left >= right) return nums[left];

        // 随机选择 pivot 避免退化
        int pivot_index = left + rand() % (right - left + 1);
        int pivot_final = partition(nums, left, right, pivot_index);

        if (pivot_final == k_smallest) {
            return nums[pivot_final];
        } else if (pivot_final > k_smallest) {
            return quickSelect(nums, left, pivot_final - 1, k_smallest);
        } else {
            return quickSelect(nums, pivot_final + 1, right, k_smallest);
        }
    }

    // 不使用 swap，采用左右移动方式分区
    int partition(vector<int>& nums, int left, int right, int pivot_index) {
        int pivot = nums[pivot_index];
        nums[pivot_index] = nums[left]; // 将 pivot 移到最左边，保存其值
        int i = left;
        int j = right;

        while (i < j) {
            while (i < j && nums[j] >= pivot) j--; // 从右向左找小于 pivot 的
            nums[i] = nums[j];                     // 右侧元素填到左边空位

            while (i < j && nums[i] <= pivot) i++; // 从左向右找大于 pivot 的
            nums[j] = nums[i];                     // 左侧元素填到右边空位
        }

        nums[i] = pivot; // 最后将 pivot 填入正确位置
        return i;        // 返回 pivot 的最终位置
    }
};
